randmain: randmain.o randcpuid.o
	$(CC) $(CFLAGS) -ldl -Wl,-rpath=$(PWD) $^ -o $@

#Static libs                                                                  
randcpuid.o: randcpuid.c randcpuid.h
	$(CC) $(CFLAGS) -c $< -o $@

randmain.o: randmain.c randcpuid.h
	$(CC) $(CFLAGS) -c $< -o $@

#Dynamic libs                                                                

        #.so's                                                               
randlibsw.so: randlibsw.o
	$(CC) $(CFLAGS) -shared $< -o $@

randlibhw.so: randlibhw.o
	$(CC) $(CFLAGS) -shared $< -o $@

        #.o's                                                                
randlibsw.o: randlibsw.c randlib.h
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

randlibhw.o: randlibhw.c randlib.h
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

