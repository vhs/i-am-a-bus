// #undef INTERFACE
#if !defined(IAMABUS_WEBSERVER_H)

void network_init();
void network_loop();
void network_start();
void network_stop();

bool network_is_up();

#endif
