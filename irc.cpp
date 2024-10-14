/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acanavat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:14:07 by acanavat          #+#    #+#             */
/*   Updated: 2024/10/14 17:51:58 by rbulanad         ###   ########.fr       */
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

void Client::setNickname(std::string newNickname)
{
	this->nickname = newNickname;
}

std::string Client::getNickname() const 
{
	return  this->nickname;
}

int Client::getFd() const
{
	return (this->fd);
}
bool Client::operator==(const Client &first)
{
	if (this->nickname == first.getNickname() && this->fd == first.getFd())
		return true;
	return false;
}

bool Client::operator!=(const Client &first)
{
	if (*this == first)
		return false;
	return true;
}

Channel::Channel()
{
	this->cmdL = false;
	this->mdp = false;
	this->topic_switch = false;
}
Channel::~Channel()
{
}
bool Channel::getCmdl()
{
	return this->cmdL;
}
void Channel::setCmdl(bool cmd)
{
	this->cmdL = cmd;
}
int Channel::getLimitl()
{
	return this->limitL;
}
void Channel::setLimitl(int newlimit)
{
	this->limitL = newlimit;
}
std::string Channel::getCmdk()
{
	return this->cmdK;
}
void Channel::setCmdk(std::string cmd)
{
	this->mdp = true;
	this->cmdK = cmd;
}
bool Channel::channelMdp()
{
	if (mdp)
		return true;
	return false;
}
void Channel::deleteMdp()
{
	setCmdk("");
	this->mdp = false;
}
std::string Channel::getTopic()
{
	std::cout << this->topic << std::endl;
	return this->topic;
}
void Channel::setTopic(std::string new_topic, Client topic_client)
{
	if (!getTopicswitch())
	{
	for (std::vector<Client *>::iterator it = this->clientOperator.begin(); it != this->clientOperator.end(); it++)
	{
		if (*(*it) == topic_client)
		{
			std::cout << "topic changed by " << topic_client.getNickname() << std::endl;
			this->topic = new_topic;
			return ;
		}
	}
	}
	else
		this->topic = new_topic;
	topic_client.sendMsg("you don't have permission for this\n");
}

std::vector<Client *> Channel::getClientlist()
{
	return this->clientList;
}

int Channel::nbrClientlist()
{
	return this->clientList.size();
}

void Channel::addClientlist(Client *newClient)
{
	std::vector<Client *>::iterator it = this->clientList.begin();

	if (!newClient)
		return ;
	for (;it != this->clientList.end() && (*it) != newClient; it++);
	
	if (it != this->clientList.end() && (*it) == newClient)
		return ;
	this->clientList.push_back(newClient);
}
void Channel::removeClientlist(Client adiosClient)
{
	for (std::vector<Client *>::iterator it = this->clientList.begin(); it != this->clientList.end(); it++)
	{
		if (*(*it) == adiosClient)
		{
			this->clientList.erase(it);
			return ;
		}
	}
}
std::vector<Client *> Channel::getClientoperator()
{
	return this->clientOperator;
}
void Channel::addClientoperator(Client *newClient)
{
	std::vector<Client *>::iterator it = this->clientOperator.begin();

	if (!newClient)
		return ;
	for (;it != this->clientOperator.end() && (*it) != newClient; it++);
	
	if (it != this->clientOperator.end() && (*it) == newClient)
		return ;
	this->clientOperator.push_back(newClient);
}
void Channel::removeClientoperator(Client adiosClient)
{
	for (std::vector<Client *>::iterator it = this->clientOperator.begin(); it != this->clientOperator.end(); it++)
	{
		if (*(*it) == adiosClient)
		{
			this->clientList.erase(it);
			return ;
		}
	}
}
std::vector<Client *> Channel::getClientcreator()
{
	return this->clientCreator;

}
void Channel::addClientcreator(Client *newClient)
{
	this->clientCreator.push_back(newClient);
	this->clientOperator.push_back(newClient);

}
void Channel::removeClientcreator(Client adiosClient)
{
	for (std::vector<Client *>::iterator it = this->clientCreator.begin(); it != this->clientCreator.end(); it++)
	{
		if (*(*it) == adiosClient)
		{
			this->clientList.erase(it);
			return ;
		}
	}
}
void Channel::setShortname(std::string name)
{
	this->shortName = name;
}

std::string Channel::getShortname()
{
	return this->shortName;
}
void Channel::setTopicswitch(bool new_switch)
{
	this->topic_switch = new_switch;
}
bool Channel::getTopicswitch()
{
	return this->topic_switch;
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
void createChannel(std::string channel_name, Client *create_channel, std::map<std::string, Channel*> &to_edit)
{
	Channel *newChannel = new(Channel);
	newChannel->setShortname(channel_name);
	newChannel->addClientcreator(create_channel);
	newChannel->addClientoperator(create_channel);
	newChannel->addClientlist(create_channel);
	to_edit[newChannel->getShortname()] = newChannel;
}

void destroyChannel(std::string channel_name, std::map<std::string, Channel*> &channel_list)
{
	for(std::map<std::string, Channel*>::iterator it = channel_list.begin(); it != channel_list.end(); it++)
	{
		if ((*it).first == channel_name)
		{
			delete((*it).second);
			channel_list.erase((*it).first);
			return ;
		}
	}
}


void joinChannel(Client *join_channel, Channel *to_edit)
{
	if (!(to_edit->getCmdl()))
		to_edit->addClientlist(join_channel);
	else if (to_edit->getLimitl() >= to_edit->nbrClientlist())
	{
		to_edit->addClientlist(join_channel);
	}
	else
		std::cout << "serveur full" << std::endl;
}

void Client::sendMsg(std::string msg)
{
	write(this->fd, msg.c_str(), msg.size());
}

void Channel::msgChannel(Client sender, std::string msg)
{
	size_t x = 0;
	for (;this->clientList.size() >= x; x++)
	{
		if (*this->clientList[x] != sender)
			this->clientList[x]->sendMsg(msg);
	}
}

void Channel::leaveChannel(Client leave)
{

	for(std::vector<Client *>::iterator it = this->clientList.begin(); it != this->clientList.end(); it++)
	{
		if (*(*it) == leave)
		{
			this->clientList.erase(it);
			return ;
		}
	}
}

void printChannel(std::map<std::string, Channel*> &channel_list)
{
	for(std::map<std::string, Channel*>::iterator it = channel_list.begin(); it != channel_list.end(); it++)
	{
		std::cout << "Server name : " << (*it).first << std::endl;
	}
}

Channel *getChannel(std::string name, std::map<std::string, Channel*> *channel_list)
{
	for(std::map<std::string, Channel*>::iterator it = channel_list->begin(); it != channel_list->end(); it++)
	{
		if (it->first == name)
			return (it->second);
	}
	return (NULL);
}
void printclientList(Channel client_list)
{
	std::vector<Client *> print;
	print = client_list.getClientlist();
	for(size_t x = 0; x < print.size(); x++)
	{
		std::cout << print[x]->getNickname() << std::endl;
	}
}

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
    std::map<std::string, Channel*> channel_list;
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

	client.push_back(create_pollfd(server_fd, (POLLIN|POLLOUT), 0));
/*	Client *fake_client = new Client();
	//fake_client->setNickname("fake_client");
	Client *fake = new Client();
	//fake->setNickname("fake");
	fake->setFd(-1);
	fake_client->setFd(-2); // Un fd fictif pour un client factice

	// Création de deux canaux manuellement
		createChannel("channel1", fake_client, channel_list);
		//channel_list["channel1"]->setLimitl(0);
		//channel_list["channel1"]->setCmdl(true);
		createChannel("channel2", fake_client, channel_list);
		createChannel("test", fake, channel_list);
	    joinChannel(fake, getChannel("channel1", &channel_list));  // Utilisez it->second pour accéder au Channel*
		//printChannel(channel_list);
		std::map<std::string, Channel*>::iterator ite = channel_list.begin();
		printclientList(*(*ite).second);
		channel_list["channel1"]->leaveChannel(*fake);
		printclientList(*(*ite).second);
		channel_list["channel1"]->setCmdk("ZOZ");
		channel_list["channel1"]->deleteMdp();
		channel_list["channel1"]->channelMdp();
		channel_list["channel1"]->setTopicswitch(true);
		channel_list["channel1"]->setTopic("new topic", *fake);
		channel_list["channel1"]->getTopic();
		return 1;*/
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
	try
	{
		Parser(msg);
		(void)client;
	}
	catch (std::string msg)
	{
		std::cerr << msg << std::endl;
	}
}

void	Parser(std::string cmd) //isole les commandes avec les arguments
{
	int y = 0;
	for (int i = 0; cmd[i]; i++)
	{
		if (cmd[i] == '\n' && cmd[i-1] == '\r')
		{
			CmdParser(cmd.substr(y, i - y));
			y = i + 1;
		}
	}
}

void	CmdParser(std::string cmd)
{
	cmd = cmd.erase(cmd.size() - 1);
	std::cout << "PARSED CMD = " << cmd << std::endl;
	if (cmd.find(' ') == std::string::npos)
	{
		const char *msg = "CMD is missing arguments \n";
		throw (msg);
	}

	std::stringstream ss(cmd);
	std::string word;
	while (!ss.eof())
	{
		getline(ss, word, ' ');
		if (word == "CAP")
			std::cout << "IGNORE CAP LS" << std::endl;
	}
}
