#ifndef _UDS_CLIENT_TASK_H_
#define _UDS_CLIENT_TASK_H_

int uds_send(unsigned char *data, unsigned int len);
void uds_client_task(void);
int uds_client_init(void);
void uds_client_close(void);
#endif