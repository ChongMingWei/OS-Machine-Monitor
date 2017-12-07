1.server.c
    a.structure
        i)void *connection_handler(void *):use the multi-thread and select funtions here.
        ii)void return_pid(char *result):return all process' ids by result.
        iii)void return_thread(char *result,char *number):return the selected process's threads.
        iv)void return_child(char *result,char *number):return the selected process's childs.
        v) return_name(char *result,char *number):return the selected process's name.
        vi) return_state(char *result,char *number):return the selected process's state.
        vii) return_cmdline(char *result,char *number):return the selected process's cmdline.
        viii) parent_Pid(char *result,char *number):return the selected process's PPid.
        ix) return_ancient(char *result,char *number):return the selected process's ancient.
        x) return_VmSize(char *result,char *number):return the selected process's virtual memory size.
        xi) return_VmRss(char *result,char *number):return the selected process's physical memory size.
	b.programming idea
		i)the socket building is referenced by this website:http://www.binarytides.com/socket-programming-c-linux-tutorial/.
		ii)the structure(ii),(iii),(iv) use the <dirent.h> to read all files under the target folder. 
		iii)the structure(v),(vi),(vii),(viii),(ix),(x) use the file read method to get the imformation in "status","cmdline" and "child".
		iv)the structure(i) is resonsible for the reiceive and send tasks to the client

2.client.c
	a.structure
		i)Only one main function and a infinite loop used for questioning which function to do in it.
	b.programming idea
		i)The socket is built at the same time with server's.
		
3.exceution file
		Only client and server,the others are tested files.
