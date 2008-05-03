#ifndef __IPC_H
#define __IPC_H


/*
 * Inter Process Communication
 * ===========================
 *
 * See ``docs/IPC'' for a sample IPC converstaion.
 */



typedef struct message {
	void *data;
	unsigned int address, flag1, flag2, flag3, flag4;
} message_t;


/* ipc_send(msg)
 * Takes a message and sends it to the corresponding server process. This is an
 * asynchronous call. This call will validate the message and raise and error
 * if the message is invalid. The return value is a local port that can be
 * listened on for any server response. NOTE: if the server does not respond
 * the listen will deadlock.
 */
unsigned short
ipc_send(message_t *msg);


/* ipc_receive(msg, port)
 * This call will block until a message is received on the specified port. If
 * port is 0 a message on any port will cause this to return with that message.
 * The return value is an address that can be used to send messages back to the
 * sending process.
 */
unsigned int
ipc_recieve(message_t *msg, unsigned short port);


/* ipc_select()
 * This call will allow a server process to asynchronously determine if they
 * have any messages waiting on the specified local ports. The interface for
 * this function is yet to be determined.
 */
ipc_select();


/* ipc_data_load()
 * This function will load a page into the current processes IPC memory area.
 * This page can be used to send data along with a message. NOTE: once a
 * message is sent this page is invalid and cannot be used again.
 */
void *
ipc_data_load();


/* ipc_data_unload()
 * Given a page that has been mapped into the current process's IPC memory area
 * this will unmap that page and free the memory.
 */
void
ipc_data_unload(void *page);


/* ipc_data_head(page)
 * Given a page that has been mapped into the current process's IPC memory area
 * this will move that page into the process's heap. This pointer passed in
 * will be invaldated and a new pointer to the same page will be returned.
 */
void *
ipc_data_heap(void *page);


/* ipc_lookup(server)
 * Given a server name this function returns an address to the server's main
 * port. A server name is as follows ``/sys/<server>'', optionally with a port
 * ``/sys/<server>/<port>''.
 */
unsigned int
ipc_lookup(const char *server);


/* ipc_send_sync(msg)
 * This will send the specified message and wait for the server to respond.
 * This function blocks until the response has arrived. The server's response
 * overwrites the sending message.
 */
void
ipc_send_sync(message_t *msg) {
	unsigned short port;
	port = ipc_send(msg);
	ipc_receive(msg, port);
}


/* ipc_get_port()
 * Returns a semi-random port of the current process.
 */
unsigned short
ipc_get_port();


/* ipc_get_address()
 * Returns the address of the current process.
 */
unsigned short
ipc_get_address();


#endif
