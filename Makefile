SRCS	=   sum_mesh_from_ascii.cpp
OBJS	=	${SRCS:.cpp=.o}
CFLAGS	=   --std=c++11 # -Wall -Wextra -Werror -O0-fsanitize=address
CC		=	g++
RM		=	-rm	-f
HEAD	=	
NAME	=	out

%.o: %.cpp $(HEAD)
	$(CC) ${CFLAGS} -c $< -o ${<:.cpp=.o}

${NAME}: ${OBJS}
		$(CC) ${CFLAGS} ${OBJS} -o ${NAME}

all: ${NAME}

clean:
	rm -f $(OBJS)
	rm *.replace

fclean:	clean
	rm -f $(NAME)

re:		fclean all

.PHONY:	all bonus clean fclean re
