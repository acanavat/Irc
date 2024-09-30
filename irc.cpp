/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acanavat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:14:07 by acanavat          #+#    #+#             */
/*   Updated: 2024/09/12 17:14:15 by acanavat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

Client::Client()
{
	this->fd = -1;
	this->username = "default";
	this->nickname = "default";
	this->waitingRoom = "";
}

Client::~Client()
{
}

void Client::setFd(int new_fd)
{
	this->fd = new_fd;
}

int Client::getFd()
{
	return (this->fd);
}

pollfd create_pollfd(int fd, short events, short revents)
{
	pollfd init;
	init.fd = fd;
	init.events = events;
	init.revents = revents;
	return (init);
}
//premier argument = definit famille d'adresse. AF_INET = pour ipv4
//deuxieme argument = definit type de socket. SOCK_STREAM = pour TCP
//troisieme argument = definit le protocole utilise par socket. IPPROTO_TCP = protocole TCP

int main(int argc, char **argv)
{
	int server_fd;
	int port;
	int opt = 1;
	struct sockaddr_in address; //represante adresse internet pour ipv4
    int size_addr = sizeof(address);
    char buffer[BUFFER_SIZE];
    std::vector<pollfd> client;
    std::map<int, Client*> client_map;
	if (argc < 3)
	{
		return 1;
	}
    std::string password = argv[2];  
	port = atoi(argv[1]);
	server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_fd < 0)
	{
		std::cout << "Error : creation de socket impossible" << std::endl;
		return 1;
	}
	// pour reutiliser le meme port
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &opt, sizeof(opt)); 
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET; // pour ipV4
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port); // conversion port en ordre reseau 
    if (bind(server_fd, (struct sockaddr *)&address, size_addr) < 0) 
    {
        std::cout << "Error : echec entre le socket et bind" << std::endl;
        return 1;
    }
    if (listen(server_fd, 7) < 0)
   	{
        std::cout << "Error : listen" << std::endl;
        return 1;
    }

   // int new_socket;

    //new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&size_addr);
    // if (new_socket < 0)
    // {
    //     std::cerr << "Error : desole les tn c'est pas possible" << std::endl;
    //     return 1;
    // }
	client.push_back(create_pollfd(server_fd, (POLLIN|POLLOUT), 0));
	while (1)
	{
		if (poll(&(*client.begin()), client.size(), 0) < 0)
		{
        	std::cerr << "Error : poll est mourant" << std::endl;
		}
		//iterateur pour voir si evenement en cours
		for (std::vector<pollfd>::iterator it = client.begin(); it != client.end(); it++)
		{
			if (((*it).revents & POLLIN) != 1) //* pour la valeur
				continue ;
			if ((*it).fd == server_fd) // new connection
			{
				int new_socket;
   				new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&size_addr);
				if (new_socket < 0)
				{
				    std::cerr << "Error : desole les tn c'est pas possible" << std::endl;
				    return 1;
				}

				client.push_back(create_pollfd(new_socket, (POLLIN|POLLOUT), 0)); // remplie le vector de notre fd
				std::cout << "New client is here" << std::endl;
				Client *new_client = new(Client);
				new_client->setFd(new_socket);
				client_map[new_socket] = new_client;
				send(new_socket, "Bienvenue sur mon serveur\n", 26, 0);
				// const char* request = "Veuillez entrer le mot de passe : ";
				// send(new_socket, request, strlen(request), 0);
			 //    int size = read(new_socket, buffer, 1024);
			 //    buffer[size] = '\0';
				// std::string string_buffer(buffer, size);
				// string_buffer.erase(size - 1, 1);
				// string_buffer[size - 1] = '\0';
				// std::cout << string_buffer << std::endl;
				// if (string_buffer != password)
				// {
			 //        send(new_socket, "Error: bad password\n", 20, 0);
				// }
				break ; 
			}
			else
			{
				memset(buffer, 0, BUFFER_SIZE);
				int n = recv((*it).fd, buffer, BUFFER_SIZE - 1, 0);
				if (n < 0)
				{
					std::cerr << "Error : recv" << std::endl;
					return 1;
				}
				if (n > 0)
				{
					std::cout << "Message : " << buffer;
					client_map[(*it).fd]->waitingRoom += std::string(buffer);
					if (strchr(client_map[(*it).fd]->waitingRoom.c_str(), '\n'))
					{
						rattrapeReddy(client_map[(*it).fd]->waitingRoom, client_map[(*it).fd]);
						client_map[(*it).fd]->waitingRoom = "";
					}
				}	
				else //if (n == 0) //deco client
				{
					delete(client_map[(*it).fd]);
					client.erase(it);
					std::cout << "Client disconected" << std::endl;
				}
				break ;
			}
		}
	}
}

void rattrapeReddy(std::string msg, Client *client)
{
	std::cout << "message recu : " << msg << std::endl;
	(void)client;
	(void)msg;
}
