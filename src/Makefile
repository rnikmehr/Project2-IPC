cc = g++  # use g++ compiler
path = ..
standard = c++17

flags = -std=$(standard)  # compile with standard
flags += -Wall  # compile with all warnings
flags += -pedantic  # even more warnings
flags += -I $(path)  # add parent to preprocessor include path
flags += -g  # include gdb instrumentation

link = $(cc) $(flags) -o  # final linked build to binary executable
compile = $(cc) $(flags) -c -o  # compilation to intermediary .o files


# Executables
text-server : text-server.o
	$(link) $@ $^

# Objects
text-server.o : text_server.cc domain_socket_server.cc
	$(compile) $@ $<


# Executables
text-client : text-client.o
	$(link) $@ $^

# Objects
text-client.o : text_client.cc domain_socket_client.cc
	$(compile) $@ $<

clean:
	$(RM) *.o text-server text-client
