CC = $(CROSSCOMPILE)gcc
STRIP = $(CROSSCOMPILE)strip
LDFLAGS = -L../../libcgi-1.0/src -lsqlite3 -lcgi
INCLUDE = -I../../libcgi-1.0/src
all: login.cgi logout.cgi sort.cgi insert.cgi delete.cgi update.cgi
%.cgi: %.c
	$(CC) $(INCLUDE) $(LDFLAGS) $< -o $@
	$(STRIP) $@
clean:
	rm *.cgi
install:
	cp *.cgi *.html /home/akaedu/rootfs/srv/www
