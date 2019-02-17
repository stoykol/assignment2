server: server.c
	gcc server.c -o a.out
client: client.c
	gcc client.c -o b.out
newclient: newclient.c
	gcc newclient.c -o c.out
