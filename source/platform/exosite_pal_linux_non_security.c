#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <assert.h>

#include <platform/exosite_pal.h>

#define timeradd(a, b, result) \
		(result)->tv_sec = (a)->tv_sec + (b)->tv_sec; \
		(result)->tv_usec = (a)->tv_usec + (b)->tv_usec; \
		if ((result)->tv_usec >= 1000000) \
		{ \
			++(result)->tv_sec; \
			(result)->tv_usec -= 1000000; \
		} \

#define timersub(a, b, result) \
		(result)->tv_sec = (a)->tv_sec - (b)->tv_sec; \
		(result)->tv_usec = (a)->tv_usec - (b)->tv_usec; \
		if ((result)->tv_usec < 0) \
		{ \
			--(result)->tv_sec; \
			(result)->tv_usec += 1000000; \
		} \

static bool_t
    parse_date(	const char *parseData,
				int dataLen,
				char *value);

void
	exosite_pal_init()
{
}

bool_t
	exosite_pal_sock_connect(void *sock)
{
	struct sockaddr_in servAddr;
	//struct hostent *server;
	int *sockIns = (int *)sock;

    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(80);
    servAddr.sin_addr.s_addr = inet_addr("52.8.0.240");

   	*sockIns = socket(AF_INET, SOCK_STREAM, 0);

    if(connect(*sockIns, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        fprintf(stderr, "connect failed\r\n");
        return FALSE;
    }

	return TRUE;
}

bool_t
    exosite_pal_sock_is_connected(void *sock)
{
    int error = 0;
    int *sockfd = (int *)sock;
    socklen_t len = sizeof(error);

    return ((getsockopt(*sockfd, SOL_SOCKET, SO_ERROR, &error, & len) == 0) ? TRUE : FALSE);
}

bool_t
    exosite_pal_sock_read(  void *sock,
                            char *data,
                            int *dataLen)
{
    int *sockfd = (int *)sock;

    bzero(data, *dataLen);

    *dataLen = read(*sockfd, data, *dataLen);

    return ((*dataLen) > 0 ? TRUE : FALSE);
}

bool_t
    exosite_pal_sock_write( void *sock,
                            const char *data,
                            int dataLen)
{
    int *sockfd = (int *)sock;

    return ((write(*sockfd, data, dataLen) > 0) ? TRUE : FALSE);
}

void
	exosite_pal_sock_close(void *sock)
{
	int *sockfd = (int *)sock;
	close(*sockfd);
}

bool_t
	exosite_pal_load_cik(	char *cik,
							int cikLen)
{
	FILE *fp;

	if(!(fp = fopen("cik", "rb")))
	{
		return FALSE;
	}

	if(fgets(cik, cikLen + 1, fp) == NULL)
		return FALSE;

	if(strlen(cik) != cikLen)
	{
		fclose(fp);
		return FALSE;
	}

	fclose(fp);

	return TRUE;
}

void
	exosite_pal_save_cik(	const char *cik,
							int cikLen)
{
	FILE *fp;
	int cnt = 0;

	if(!(fp = fopen("cik", "wb")))
		return;

	while(cnt < cikLen)
	{
		fputc(*(cik + cnt), fp);
		++cnt;
	}

	fclose(fp);
}

void
	exosite_pal_remove_cik()
{
	remove("cik");
}

bool_t
	exosite_pal_timer_expired(exosite_timer_t *timer)
{
	struct timeval now, res;
	gettimeofday(&now, NULL);
	timersub(&timer->endTime, &now, &res);		
	return res.tv_sec < 0 || (res.tv_sec == 0 && res.tv_usec <= 0);
}


void
	exosite_pal_timer_countdown_ms(	exosite_timer_t *timer,
									unsigned int timeout)
{
	struct timeval now;
	gettimeofday(&now, NULL);
	struct timeval interval = {timeout / 1000, (timeout % 1000) * 1000};
	timeradd(&now, &interval, &timer->endTime);
}


void
	exosite_pal_timer_countdown(exosite_timer_t *timer,
								unsigned int timeout)
{
	struct timeval now;
	gettimeofday(&now, NULL);
	struct timeval interval = {timeout, 0};
	timeradd(&now, &interval, &timer->endTime);
}


int
	exosite_pal_timer_left_ms(exosite_timer_t *timer)
{
	struct timeval now, res;
	gettimeofday(&now, NULL);
	timersub(&timer->endTime, &now, &res);
	//printf("left %d ms\n", (res.tv_sec < 0) ? 0 : res.tv_sec * 1000 + res.tv_usec / 1000);
	return (res.tv_sec < 0) ? 0 : res.tv_sec * 1000 + res.tv_usec / 1000;
}


void
	exosite_pal_timer_init(exosite_timer_t *timer)
{
	timer->endTime = (struct timeval){0, 0};
}

bool_t
	exosite_pal_get_current_date_time(date_time_t *curDateTime)
{
	char *pdu = "GET /ip HTTP/1.1\r\n"
				"Host: m2.exosite.com\r\n"
				"Accept: application/x-www-form-urlencoded; charset=utf-8\r\n\r\n";
	int sock;
	char buf[128];
	int bufLen;

	if(!exosite_pal_sock_connect(&sock))
	{
		return FALSE;
	}

    if(!exosite_pal_sock_write( &sock,
                                pdu,
                                strlen(pdu)))
    {
        return FALSE;
    }


	printf("wait get RSP\r\n");
	while(1)
	{
		bufLen = sizeof(buf);
		if(exosite_pal_sock_read(	&sock,
									buf,
		                        	&bufLen)  > 0)
			break;
	}

	if(!parse_date(	buf,
					bufLen,
					curDateTime->toString))
		return FALSE;

	printf("date = %s\r\n", curDateTime->toString);
	//assert(FALSE);

	exosite_pal_sock_close(&sock);

	return TRUE;
}

bool_t
    parse_date(	const char *parseData,
				int dataLen,
				char *value)
{
    char *keyStart;
    char *valueStart;
    char *valueEnd;
    char *index;
	const char *key = "Date";

    if(value == NULL)
    {
        return FALSE;
    }

    keyStart = strstr(parseData, key);

    if(keyStart == NULL)
    {
        return FALSE;
    }

    valueStart = strstr(keyStart, ":");

    if(valueStart == NULL)
    {
        return FALSE;
    }

    valueEnd = strstr(valueStart, "GMT");

    if(valueEnd == NULL)
    {
        return FALSE;
    }

    for(index = valueStart + 2; index < valueEnd + 3; ++index)
    {
        *value = *index;

        ++value;
    }

    *value = 0;

    return TRUE;
}

