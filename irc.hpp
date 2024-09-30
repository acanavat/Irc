/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acanavat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 15:56:32 by acanavat          #+#    #+#             */
/*   Updated: 2024/09/30 15:56:34 by acanavat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP 
# define IRC_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> 
#include <cstring>
#include <stdlib.h>
#include <poll.h>
#include <vector>
#include <map>
#define BUFFER_SIZE 1024

class Client
{
	public :
	Client();
	~Client();
	void setFd(int new_fd);
	int getFd();
	std::string waitingRoom;
	private :
	std::string username;
	std::string nickname;
	int fd;
};

void rattrapeReddy(std::string msg, Client *client);
#endif 