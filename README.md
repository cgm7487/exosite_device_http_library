# Exosite Device Http Library

This is the C library for interacting with the Exosite One Platform. It aims to
be fast, efficient, and flexible. Platform operations are queued up and then
performed with non-blocking calls so that your application can continue to do
local processing while waiting for the network transactions to complete.

## Status

Because this is an alpha release, please don't use it in production.
Backwards-incompatible changes will very likely have to be made when bugs are
found, but you are highly encouraged to use it for testing. The examples do
work and the changes to the interface won't be too substantial.

Currently, activate, reads, writes, subscribes(only support single subscribe now), and content interactive functions are all workable.
You can find more informations of Exosite http API from: http://docs.exosite.com

The inline documentation is incomplete and is probably wrong in many places,
don't pay too much attention to it. You should be able to figure out most of the
usage from the examples and the function prototypes.

## About

This library uses the concept of "operations" (ops) for all interactions with
the OneP. One operation is a single read, write, or subscription of a single
dataport.

### Most Common Use

You must call "exo_init()" function at first time. After that, you can
register your read/write actions in multiple times and subscribe operation at once before calling "exo_loop_start()".

If requiring a new CIK to activate, activate operation will start automatically, so you don't need to call the activate function by yourself.

After calling "exo_loop_start()", the function will start to do all operations that you registered.
The final result of each operation will callback to you via your callback function, so you can get the final opeartion status and value.

When the "exo_loop_start()" finished, you can continue registering your new operations.

### When Not Using Provisioning with Examples

If you're planning on testing the included example and you're not using the
provisioning system you'll need to paste your CIK into a file named `cik` to
fool the library into thinking that it already activated itself.
