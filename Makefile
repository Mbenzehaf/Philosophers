# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mben-zeh <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/07 00:13:16 by mben-zeh          #+#    #+#              #
#    Updated: 2023/02/07 00:13:19 by mben-zeh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo


CC=cc

CFLAGS= -Wall -Werror -Wextra

SRC =  philo.c  libft_utils.c 

#SRC_bonus = 

OBJ=${SRC:.c=.o}

#OBJ_bonus=${SRC_bonus:.c=.o}

all:${NAME}
	
${NAME}:${OBJ}
	@$(CC) $(OBJ) -o ${NAME} -fsanitize=thread

#bonus : $(NAME_BONUS)

#${NAME_BONUS}:$(OBJ_bonus)
#	@$(CC) $(SRC_bonus) -o ${NAME_BONUS} 
	
clean :
	rm -f ${OBJ} 
#${OBJ_bonus}

fclean : clean
	rm -f ${NAME}
#${NAME_BONUS}

re : fclean all

.PHONY: all clean fclean re