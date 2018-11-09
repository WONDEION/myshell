

INC = -I ./readline/head -I ./readline/charanalysis/head -I ./main -I ./io/head \
	  -I ./msh_command/head -I ./Linux_command/head
GDB = -ggdb3

myshell : ./main/main.o ./readline/obj/Datastruct.o ./readline/obj/readline.o \
		  ./io/obj/selflib.o ./readline/charanalysis/obj/charanalysis.o \
		  ./readline/obj/initialize.o ./msh_command/obj/history.o \
		  ./readline/obj/signalcontrol.o ./readline/obj/process.o \
		  ./Linux_command/obj/analysis.o ./Linux_command/obj/redirect.o \
		  ./Linux_command/obj/linuxcommand.o ./io/obj/termiocontrol.o \
		  ./msh_command/obj/shellcommand.o ./readline/obj/misstake.o \
		  ./msh_command/obj/command_cd.o ./readline/obj/press_tab.o \
		  ./msh_command/obj/command_source.o ./Linux_command/obj/execfile.o \
		  ./msh_command/obj/command_alias.o ./msh_command/obj/command_unalias.o \
		  ./Linux_command/obj/multiple_command.o ./msh_command/obj/command_jobs.o \
		  ./msh_command/obj/command_type.o ./msh_command/obj/command_exit.o \
		  ./msh_command/obj/command_let.o \

		  gcc $(GDB) ./main/main.o ./readline/obj/Datastruct.o ./readline/obj/readline.o \
		             ./io/obj/selflib.o ./readline/charanalysis/obj/charanalysis.o \
		             ./readline/obj/initialize.o ./msh_command/obj/history.o \
		             ./readline/obj/signalcontrol.o ./readline/obj/process.o \
		             ./Linux_command/obj/analysis.o ./Linux_command/obj/redirect.o \
		             ./Linux_command/obj/linuxcommand.o ./io/obj/termiocontrol.o \
		             ./msh_command/obj/shellcommand.o ./readline/obj/misstake.o \
		             ./msh_command/obj/command_cd.o ./readline/obj/press_tab.o \
		             ./msh_command/obj/command_source.o ./Linux_command/obj/execfile.o \
		             ./msh_command/obj/command_alias.o ./msh_command/obj/command_unalias.o \
		             ./Linux_command/obj/multiple_command.o ./msh_command/obj/command_jobs.o \
		             ./msh_command/obj/command_type.o ./msh_command/obj/command_exit.o \
		             ./msh_command/obj/command_let.o \
		             -o myshell

./main/main.o : ./main/main.c ./main/main.h ./readline/head/readline.h \
   				./readline/head/Datastruct.h ./readline/head/initialize.h \
   				./io/head/selflib.h ./msh_command/head/history.h \
   				./Linux_command/head/analysis.h 

				gcc $(GDB) $(INC) -c ./main/main.c \
								  -o ./main/main.o

./readline/obj/readline.o : ./readline/source/readline.c ./readline/head/readline.h \
							./readline/head/Datastruct.h ./readline/head/misstake.h \
							./io/head/selflib.h

							gcc $(GDB) $(INC) -c ./readline/source/readline.c \
											  -o ./readline/obj/readline.o

./readline/obj/Datastruct.o : ./readline/source/Datastruct.c ./io/head/selflib.h
	
							  gcc $(GDB) $(INC) -c ./readline/source/Datastruct.c \
							  		          	-o ./readline/obj/Datastruct.o

./io/obj/selflib.o : ./io/source/selflib.c ./io/head/selflib.h ./readline/head/misstake.h \
					 ./readline/charanalysis/head/charanalysis.h
					
					 gcc $(GDB) $(INC) -c ./io/source/selflib.c \
					  				   -o ./io/obj/selflib.o

./readline/charanalysis/obj/charanalysis.o : ./readline/charanalysis/source/charanalysis.c \
											 ./readline/charanalysis/head/charanalysis.h \
											 ./readline/head/Datastruct.h \
											 ./io/head/selflib.h \
											 ./msh_command/head/history.h \
											 ./readline/head/press_tab.h \
											 ./msh_command/head/command_alias.h \

											 gcc $(GDB) $(INC) -c ./readline/charanalysis/source/charanalysis.c \
															   -o ./readline/charanalysis/obj/charanalysis.o

./readline/obj/initialize.o : ./readline/source/initialize.c ./readline/head/readline.h \
							  ./readline/head/Datastruct.h ./readline/head/initialize.h \
							  ./msh_command/head/history.h ./readline/head/signalcontrol.h \
							  ./readline/head/process.h ./io/head/termiocontrol.h \
							  ./msh_command/head/command_alias.h \
							  ./Linux_command/head/analysis.h \
							  
							  gcc $(GDB) $(INC) -c ./readline/source/initialize.c \
							  					-o ./readline/obj/initialize.o

./msh_command/obj/history.o : ./readline/head/Datastruct.h ./msh_command/head/history.h \
							  ./msh_command/source/history.c ./io/head/selflib.h 

							  gcc $(GDB) $(INC) -c ./msh_command/source/history.c \
							  					-o ./msh_command/obj/history.o

./readline/obj/signalcontrol.o : ./readline/head/signalcontrol.h \
								 ./readline/source/signalcontrol.c \
								 ./readline/head/Datastruct.h  ./io/head/selflib.h \
								 ./readline/head/process.h \
								 ./readline/head/readline.h \

								 gcc $(GDB) $(INC) -c ./readline/source/signalcontrol.c \
								 				   -o ./readline/obj/signalcontrol.o 										

./readline/obj/process.o : ./readline/source/process.c ./readline/head/process.h \
						   ./readline/head/Datastruct.h ./io/head/selflib.h \
						   ./readline/head/signalcontrol.h

						   gcc $(GDB) $(INC) -c ./readline/source/process.c \
						   					 -o	./readline/obj/process.o

./Linux_command/obj/analysis.o : ./Linux_command/source/analysis.c \
								 ./Linux_command/head/analysis.h \
								 ./readline/head/Datastruct.h ./io/head/selflib.h \
								 ./readline/head/process.h ./msh_command/head/history.h \
								 ./Linux_command/head/redirect.h \
								 ./Linux_command/head/linuxcommand.h \
								 ./msh_command/head/shellcommand.h \
								 ./Linux_command/head/execfile.h \
								 ./msh_command/head/command_alias.h \
								 ./Linux_command/head/multiple_command.h \

								 gcc $(GDB) $(INC) -c ./Linux_command/source/analysis.c \
								 				   -o ./Linux_command/obj/analysis.o

./Linux_command/obj/redirect.o :  ./Linux_command/head/redirect.h \
								  ./Linux_command/source/redirect.c \
								  ./readline/head/Datastruct.h ./io/head/selflib.h \
								  ./msh_command/head/command_cd.h \
								  ./Linux_command/head/analysis.h \
								  ./main/main.h \
								  ./io/head/termiocontrol.h \

								   gcc $(GDB) $(INC) -c ./Linux_command/source/redirect.c \
								 				   -o ./Linux_command/obj/redirect.o

./Linux_command/obj/linuxcommand.o : ./Linux_command/source/linuxcommand.c \
									 ./Linux_command/head/linuxcommand.h \
									 ./readline/head/Datastruct.h \
									 ./io/head/selflib.h ./readline/head/process.h \
									 ./readline/head/signalcontrol.h \
									 ./Linux_command/head/execfile.h

									 gcc $(GDB) $(INC) -c ./Linux_command/source/linuxcommand.c \
									 				   -o ./Linux_command/obj/linuxcommand.o

./io/obj/termiocontrol.o : ./io/head/termiocontrol.h ./io/source/termiocontrol.c

						   gcc $(GDB) $(INC) -c ./io/source/termiocontrol.c \
						   					 -o ./io/obj/termiocontrol.o

./msh_command/obj/shellcommand.o : ./msh_command/head/shellcommand.h \
								   ./msh_command/source/shellcommand.c \
								   ./readline/head/Datastruct.h ./io/head/selflib.h \
								   ./msh_command/head/command_cd.h \
								   ./msh_command/head/history.h \
								   ./msh_command/head/command_source.h \
								   ./msh_command/head/command_alias.h \
								   ./msh_command/head/command_unalias.h \
								   ./msh_command/head/command_jobs.h \
								   ./msh_command/head/command_type.h \
								   ./msh_command/head/command_exit.h \
								   ./msh_command/head/command_let.h \

								   gcc $(GDB) $(INC) -c ./msh_command/source/shellcommand.c \
								   					 -o	./msh_command/obj/shellcommand.o

./readline/obj/misstake.o : ./readline/source/misstake.c ./readline/head/misstake.h

							gcc $(GDB) $(INC) -c ./readline/source/misstake.c \
											  -o ./readline/obj/misstake.o

./msh_command/obj/command_cd.o : ./msh_command/head/command_cd.h \
								 ./msh_command/source/command_cd.c \
								 ./readline/head/Datastruct.h ./io/head/selflib.h \
								 ./readline/head/misstake.h ./readline/head/process.h

								 gcc $(GDB) $(INC) -c ./msh_command/source/command_cd.c \
								 				   -o ./msh_command/obj/command_cd.o

./readline/obj/press_tab.o : ./readline/head/press_tab.h \
							 ./readline/source/press_tab.c \
							 ./readline/head/Datastruct.h ./io/head/selflib.h \
							 ./msh_command/head/shellcommand.h \
							 ./readline/head/Datastruct.h

							 gcc $(GDB) $(INC) -c ./readline/source/press_tab.c \
							 				   -o ./readline/obj/press_tab.o

./msh_command/obj/command_source.o : ./msh_command/head/command_source.h \
									 ./msh_command/source/command_source.c \
									 ./readline/head/Datastruct.h \
									 ./io/head/selflib.h \
									 ./msh_command/head/command_cd.h \
									 ./main/main.h

									 gcc $(GDB) $(INC) -c ./msh_command/source/command_source.c \
									 				   -o ./msh_command/obj/command_source.o

./Linux_command/obj/execfile.o : ./readline/head/Datastruct.h ./io/head/selflib.h \
								 ./Linux_command/source/execfile.c \
								 ./Linux_command/head/execfile.h \
								 ./readline/head/misstake.h \
								 ./Linux_command/head/linuxcommand.h \
								 ./msh_command/head/command_cd.h \
								 ./readline/head/signalcontrol.h \

								 gcc $(GDB) $(INC) -c ./Linux_command/source/execfile.c \
								 				   -o ./Linux_command/obj/execfile.o

./msh_command/obj/command_alias.o : ./msh_command/source/command_alias.c \
									./msh_command/head/command_alias.h \
									./readline/head/Datastruct.h \
									./io/head/selflib.h \
									./readline/head/misstake.h \
									./readline/head/process.h \

									gcc $(GDB) $(INC) -c ./msh_command/source/command_alias.c \
												      -o ./msh_command/obj/command_alias.o

./msh_command/obj/command_unalias.o : ./msh_command/source/command_unalias.c \
									  ./msh_command/head/command_unalias.h \
									  ./readline/head/Datastruct.h \
									  ./io/head/selflib.h \
									  ./readline/head/misstake.h \
									  ./readline/head/process.h \
									  ./msh_command/head/command_alias.h \

									  gcc $(GDB) $(INC) -c ./msh_command/source/command_unalias.c \
												 	    -o ./msh_command/obj/command_unalias.o

./Linux_command/obj/multiple_command.o : ./Linux_command/source/multiple_command.c \
										 ./Linux_command/head/multiple_command.h \
										 ./readline/head/Datastruct.h \
									  	 ./io/head/selflib.h \
									  	 ./readline/head/process.h \
									  	 ./Linux_command/head/execfile.h \
									  	 ./main/main.h ./Linux_command/head/analysis.h \

									  	  gcc $(GDB) $(INC) -c ./Linux_command/source/multiple_command.c \
									  	  					-o ./Linux_command/obj/multiple_command.o

./msh_command/obj/command_jobs.o : ./msh_command/source/command_jobs.c \
								   ./msh_command/head/command_jobs.h \
								   ./readline/head/Datastruct.h \
								   ./io/head/selflib.h \
								   ./readline/head/misstake.h \
								   ./readline/head/process.h \
								   ./io/head/termiocontrol.h \
								   ./readline/head/signalcontrol.h \

								   gcc $(GDB) $(INC) -c ./msh_command/source/command_jobs.c \
								   					 -o ./msh_command/obj/command_jobs.o

./msh_command/obj/command_type.o : ./msh_command/source/command_type.c \
								   ./msh_command/head/command_type.h \
								   ./readline/head/Datastruct.h \
								   ./io/head/selflib.h \
								   ./readline/head/misstake.h \
								   ./msh_command/head/shellcommand.h \
								   ./msh_command/head/command_alias.h \

								   gcc $(GDB) $(INC) -c ./msh_command/source/command_type.c \
								   					 -o ./msh_command/obj/command_type.o

./msh_command/obj/command_exit.o : ./msh_command/source/command_exit.c \
								   ./msh_command/head/command_exit.h \
								   ./readline/head/readline.h \

								   gcc $(GDB) $(INC) -c ./msh_command/source/command_exit.c \
								   					 -o ./msh_command/obj/command_exit.o

./msh_command/obj/command_let.o : ./msh_command/source/command_let.c \
								  ./msh_command/head/command_let.h \
								  ./readline/head/Datastruct.h \
								  ./io/head/selflib.h \
								  ./readline/head/misstake.h \
								  ./readline/head/process.h \

								  gcc $(GDB) $(INC) -c ./msh_command/source/command_let.c \
								  					-o ./msh_command/obj/command_let.o \