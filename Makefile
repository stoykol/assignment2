server: server.c
	gcc server.c -o a.out
client: client.c
	gcc client.c -o b.out
newclient: newclient.c
	gcc newclient.c -o c.out
aserver: aserver.c
	gcc aserver.c -o serv
bclient: bclient.c
	gcc bclient.c -o cli
newbclient: newbclient.c
	gcc newbclient.c -o newb

