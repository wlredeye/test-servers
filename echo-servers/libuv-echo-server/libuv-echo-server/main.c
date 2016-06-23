#include <uv.h>
#include <stdio.h>
#include <stdlib.h>

uv_tcp_t server;
uv_loop_t *loop;

void read_cb(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf){
    uv_write_t *req = (uv_write_t*)malloc(sizeof(uv_write_t));
    
    uv_buf_t wbuf = uv_buf_init(buf->base, nread);
    int r = uv_write(req, stream, &wbuf, 1, NULL);
    
    if (r < 0)
        fprintf(stderr, "Error on writing client stream: %s.\n", r);
    
    free(wbuf.base);
    free(req);
}

void alloc_buffer(uv_handle_t * handle, size_t size, uv_buf_t *buf) {
    buf->base = (char*)malloc(size);
    buf->len = size;
}

void connection_cb(uv_stream_t *server,int status){
    uv_tcp_t *client = malloc(sizeof(uv_tcp_t));
    
    if (status < 0)
        fprintf(stderr, "Error on listening: %s.\n", status);
    
    uv_tcp_init(loop, client);
    if (uv_accept(server, (uv_stream_t *) client) == 0) {
        int r = uv_read_start((uv_stream_t *) client, alloc_buffer, read_cb);
        if (r < 0)
            fprintf(stderr, "Error on reading client stream: %s.\n", r);
    } else
        uv_close((uv_handle_t *) client, NULL); /* close client stream on error */
}

int main(int argc, const char * argv[]) {
    loop = uv_default_loop();
    
    struct sockaddr_in addr;
    uv_ip4_addr("127.0.0.1", 12345, &addr);
    
    uv_tcp_init(loop, &server);
    uv_tcp_bind(&server, &addr, 0);
    int r = uv_listen((uv_stream_t*)&server, 128, connection_cb);
    if (r < 0 )
        return fprintf(stderr, "Error on listening: %s.\n", r);
    return uv_run(loop, UV_RUN_DEFAULT);
}