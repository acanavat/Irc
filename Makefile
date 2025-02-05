# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: acanavat <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/08 13:29:16 by acanavat          #+#    #+#              #
#    Updated: 2025/02/05 16:28:03 by rbulanad         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= ircserv
SOURCES		= irc.cpp

OBJECTS		= ${SOURCES:.cpp=.o}



RM			= rm -f
CC			= c++
CFLAGS		= -Wall -Wextra -Werror -std=c++98 -g

ifdef MAKEDEBUG
	CFLAGS += -fsanitize=thread -g
endif

ifdef MAKELEAKS
	CFLAGS += -fsanitize=leak -g
endif

all:		${NAME}

.cpp.o:
	${CC} ${CFLAGS} -c $< -o ${<:.cpp=.o}


${NAME}: 	${OBJECTS}
	${CC} ${CFLAGS} ${OBJECTS} -o ${NAME}

clean:
	${RM} ${OBJECTS} ${OBJECTS_BONUS}

fclean: clean
	${RM} ${NAME} 

re: fclean all

debug:
	make MAKEDEBUG=1 re

leaks:
	make MAKELEAKS=1 re

.PHONY: all clean fclean re
