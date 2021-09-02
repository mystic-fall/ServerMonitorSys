CC := gcc
HPATH := ../1.headfiles
CFLAGS := -lpthread
MACRO := _D

COBJS := client.o $(HPATH)/cJSON.o $(HPATH)/tcp_client.o $(HPATH)/udp_client.o $(HPATH)/common.o packageToJson.o

all: client

client: $(COBJS) 
	$(CC) $(COBJS) $(CFLAGS) -o $@ 

$(COBJS): %.o : %.c
	$(CC) -c $< -o $@ -D $(MACRO)

.PHONY: clean
clean: 	
	rm -rf ../1.headfiles/*.o ./*.o ./client
