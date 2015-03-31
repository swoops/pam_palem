CC = gcc

CFLAGS = -g -fPIC -DPIC -shared -rdynamic

TARGET = pam_palem.so
INSTALL = /lib64/security/pam_palem.so

all: $(TARGET) 

$(TARGET): pam_palem.c
	$(CC)  -m64 $(CFLAGS) -o $(TARGET) pam_palem.c

clean: 
	rm $(TARGET)

uninstall: 
	rm $(INSTALL)
	rm /etc/pam_palem.list

install: $(TARGET)
	install -o root -g root -m 0755 $(TARGET) $(INSTALL)
	install -o root -g root -m 0755 passwords.list /etc/pam_palem.list
