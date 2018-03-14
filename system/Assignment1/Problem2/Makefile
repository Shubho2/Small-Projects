
output: catfile.c mkdir.c top.c shell.c top concat md
	gcc shell.c -lreadline -o output 	
	
concat: catfile.c
	gcc catfile.c -o concat
	
md:     mkdir.c
	gcc mkdir.c -o md
	
top: top.c
	gcc top.c -o top
