#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <cyassl/ctaocrypt/settings.h>
#include <cyassl/ssl.h>

#include <exosite.h>
#include <platform/exosite_pal.h>

void write_callback(const exosite_data_port_t *,
					int opStatus);

void read_callback(	const exosite_data_port_t *,
					int opStatus);

void subscribe_callback(const exosite_data_port_t *,
						int opStatus);

int main()
{
	int sockRead, sockWrite, sockSub;
	exosite_data_port_t dataPorts[5];
	int len;
	int opStatus = -1;
	int i;
	date_time_t curDateTime;

	content_id_t idList[5];
	int listSize = 5;

	content_info_t contentInfo;

	uint8_t buf[256];
	int bufLen;

	CyaSSL_Init();
	

	exo_init(	"hackntu2016",
				"gt202",
				"1A:01:00:00:00:01");

	exosite_pal_get_current_date_time(&curDateTime);

	/*if(exo_download_list_content(	"renesas",
									"rl78g14",	
									idList,
									&listSize) == EXO_ERROR_STATUS_SUC)
	{

		for(i = 0; i < listSize; ++i)
		{
			printf("id = %s\r\n", idList[i].id);
		}
	}

	if(exo_download_get_content_info(	"renesas",
										"rl78g14",
										idList[0].id,
										&contentInfo) == EXO_ERROR_STATUS_SUC)
	{
		printf("type = %s\r\nsize = %d\r\ntimeStamp = %s\r\n", contentInfo.contentType, contentInfo.contentSize, contentInfo.updatedTimeStamp.toString);
	}

	unsigned int reuSize = contentInfo.contentSize;
	unsigned int start = 0;
	unsigned int end = sizeof(buf) - 1;
	unsigned int cnt = 0;
	while(reuSize > 0)
	{
		bufLen = sizeof(buf);
		if(exo_download_get_content("renesas",
									"rl78g14",
									idList[0].id,
									start,
					 				end,
									buf,
									&bufLen) == EXO_ERROR_STATUS_SUC)
		{
			for(i = 0; i < bufLen; ++i)
			{
				if(i % 16 == 0)
					printf("\r\n");
				printf("%x ", buf[i]);
			}

			start += bufLen;
			end = start + (sizeof(buf) - 1);
			reuSize -= bufLen;
			cnt += bufLen;

			printf("\r\nreduSize = %d\r\n", reuSize);
		}
		else
		{
			break;
		}
	}

	printf("cnt = %d\r\n", cnt);*/

	while(1)
	{
		printf("start to write data to cloud\r\n");
		sockWrite = exo_write(	"AccXRaw_ICM20648",
								"3.5",
								write_callback);

		//sockWrite = exo_write("humidity", "78.2", write_callback);

		//sockWrite = exo_write("uptime", "5", write_callback);

		//sockWrite = exo_write("temperature", "33.6", write_callback);

		/*sockWrite = exo_write(	"temp",
								"30.32",
								write_callback);
		if(sockWrite < 0)
			assert(FALSE);

		sockRead = exo_read("brightness",
							read_callback);
		sockRead = exo_read("led_ctrl",
							read_callback);

		if(sockRead < 0)
			assert(FALSE);

		sockSub = exo_subscribe("led_ctrl",
								&curDateTime,
								subscribe_callback);*/


		printf("loop start\r\n");
		exo_loop_start();

		exosite_pal_get_current_date_time(&curDateTime);

	
		printf("loop end\r\n");

		sleep(1);
	}

	return 0;
}

void
	write_callback(	const exosite_data_port_t *dataPort,
					int opStatus)
{
	printf("write status = %d\r\n", opStatus);
}

void
	read_callback(	const exosite_data_port_t *dataPort,
					int opStatus)
{
	printf("read status = %d\r\n", opStatus);

	printf("%s=%s\r\n", dataPort->alias, dataPort->value);
}

void
	subscribe_callback(	const exosite_data_port_t *dataPort,
						int opStatus)
{
	printf("subscribe status = %d\r\n", opStatus);
	printf("%s=%s\r\n", dataPort->alias, dataPort->value);
}

