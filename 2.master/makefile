CC := gcc
HPATH := ../1.headfiles
SQLPATH := /usr/include/mysql
CFLAGS := -lpthread
SQLFLAGS := -lmysqlclient
MACRO := _D

MOBJS := master.o mysql_opt.o $(HPATH)/cJSON.o $(HPATH)/tcp_server.o $(HPATH)/udp_server.o $(HPATH)/common.o $(HPATH)/epoll.o task_pool.o parseFromJson.o

all: master

master: $(MOBJS) 
	$(CC) $(MOBJS) $(CFLAGS) $(SQLFLAGS) -o $@ 

$(MOBJS): %.o : %.c
	$(CC) -c -I$(SQLPATH) $< -o $@ -D $(MACRO)

.PHONY: clean
clean: 	
	rm -rf ../1.headfiles/*.o
	rm -rf ./*.o
	rm ./master
