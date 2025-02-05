/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acanavat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:14:07 by acanavat          #+#    #+#             */
/*   Updated: 2025/02/05 15:53:29 by rbulanad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include <unistd.h>

Client::Client()
{
	this->fd = -1;
	this->nickname = "default";
	this->waitingRoom = "";
	this->_passBool = false;
	this->_userBool = false;
	this->_nickBool = false;
	this->_username = "default";
	this->_id = "default";
	this->_firstCoBool = true;
	this->_mode1 = "default";
	this->_mode2 = "default";
	this->_realname = "default";
}

Client::~Client()
{
	
}

void Client::setFd(int new_fd)
{
	this->fd = new_fd;
}

void Client::setNickname(std::string newnick) 
{
	nickname = newnick;
}

std::string Client::getNickname() const 
{
	return  this->nickname;
}

std::string Client::getUsername() const
{
	return this->_username;
}

std::string Client::getId() const
{
	return this->_id;
}

void Client::stringSetter(int i, std::string neww)
{
	if (i == 0)
		this->nickname = neww;
	else if (i == 1)
		this->_username = neww;
	else if (i == 2)
		this->_mode1 = neww;
	else if (i == 3)
		this->_mode2 = neww;
	else if (i == 4)
		this->_realname = neww;
	else if (i == 5)
		this->_id = neww;
}

void Client::boolSetter(int i, bool caca)
{
	if (i == 0)
		this->_passBool = caca;
	else if (i == 1)
		this->_nickBool = caca;
	else if (i == 2)
		this->_userBool = caca;
	else if (i == 3)
		this->_firstCoBool = caca;
}

bool	Client::getPassBool()
{
	return (this->_passBool);
}
void	Client::tryLogin()
{
	if (_userBool && _nickBool && _passBool)
	{
		sendMsg("001 " + nickname + " :Welcome to the Internet Relay Network :" + nickname + "!" + _username + "@localhost", fd); //WELCOME
		sendMsg("002 " + nickname + " :Your host is " + "IRCMammaMia (localhost), running version 42.24" + "\r", fd); //YOURHOST
		std::string id = "ID_" + nickname + "_ID";
		sendMsg("NOTICE: only 1(one) mode can be added/removed using the command MODE", getFd());
		stringSetter(5, id);
	}
}

bool Client::isLogged()
{
	if (_passBool && _userBool && _nickBool)
		return (true);
	return (false);
}

int	Client::isFirstCo()
{
	if (_firstCoBool)
		return (1);
	return (0);
}

void Client::resetLogin()
{
	boolSetter(0, false);
	boolSetter(1, false);
	boolSetter(2, false);
	boolSetter(3, true);
	stringSetter(0, "default");
	stringSetter(1, "default");
	stringSetter(2, "default");
	stringSetter(3, "default");
	stringSetter(4, "default");
	stringSetter(5, "default");
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
	this->isMdp = false;
	this->addMode('n');
	this->limit = 0;
	this->_mdp = "";
}

Channel::~Channel()
{
}

int Channel::getLimit()
{
	return this->limit;
}

void Channel::setLimit(unsigned int newLimit)
{
	this->limit = newLimit;
}

std::string Channel::getMdp()
{
	return this->_mdp;
}

void Channel::setMdp(std::string newMdp)
{
	this->isMdp = true;
	this->_mdp = newMdp;
}

void Channel::deleteMdp()
{
	setMdp("");
	this->isMdp = false;
}

std::string Channel::getTopic()
{
	return this->topic;
}

void Channel::setTopic(std::string new_topic)
{
	this->topic = new_topic;
}

std::vector<Client *> &Channel::getClientlist()
{
	return this->clientList;
}

int Channel::nbrClientlist()
{
	return this->clientList.size();
}

void Channel::addClientlist(Client *newClient)
{
	this->clientList.push_back(newClient);
}

void Channel::removeClientlist(Client *adiosClient)
{
	for (std::vector<Client *>::iterator it = this->clientList.begin(); it != this->clientList.end(); it++)
	{
		if ((*it) == adiosClient)
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

void Channel::removeClientoperator(Client *adiosClient)
{
	if (this->clientOperator.empty())
		return ;
	for (std::vector<Client *>::iterator it = this->clientOperator.begin(); it != this->clientOperator.end(); it++)
	{
		if ((*it) == adiosClient)
		{
			this->clientOperator.erase(it);
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
	std::vector<Client *>::iterator it = this->clientCreator.begin();

	if (!newClient)
		return ;
	for (;it != this->clientCreator.end() && (*it) != newClient; it++);
	if (it != this->clientCreator.end() && (*it) == newClient)
		return ;
	this->clientCreator.push_back(newClient);
}

void Channel::removeClientcreator(Client *adiosClient)
{
	for (std::vector<Client *>::iterator it = this->clientCreator.begin(); it != this->clientCreator.end(); it++)
	{
		if ((*it) == adiosClient)
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

void	Channel::addInvite(Client *client)
{
	if (std::find(clientInvitation.begin(), clientInvitation.end(), client->getNickname()) != clientInvitation.end())
		return ;
	clientInvitation.push_back(client->getNickname());
}

void	Channel::removeInvite(Client *client)
{
	std::vector<std::string>::iterator it = std::find(clientInvitation.begin(), clientInvitation.end(), client->getNickname());
	if (it == clientInvitation.end())
		return ;
	clientInvitation.erase(it);
}

bool	Channel::isInvited(Client *client)
{
	return (std::find(clientInvitation.begin(), clientInvitation.end(), client->getNickname()) != clientInvitation.end());
}

pollfd	create_pollfd(int fd, short events, short revents)
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

void Client::sendMsg(std::string msg, int private_msg)
{
	msg += "\n";
	if (private_msg == -1) 
		write(this->fd, msg.c_str(), msg.size());
	else
		write(private_msg, msg.c_str(), msg.size());
	std::cout << "MSG = "<< msg << std::endl;
}

void Channel::msgChannel(int fdSender, std::string msg, int ignore)
{
	std::vector<Client *>::iterator it = clientList.begin(); 
	for (; it != clientList.end(); it++)
	{
		if (ignore && fdSender == (*it)->getFd())
			continue;
		else
		(*it)->sendMsg(msg, (*it)->getFd());
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

Client *Channel::findClient(std::string name)
{
	std::vector<Client *>::iterator it = clientList.begin();
	for(; it != clientList.end(); it++)
	{
		if ((*it)->getNickname() == name)
			return (*it);
	}
	return (NULL);
}

void	Channel::removeClient(Client *client)
{
	if (std::find(clientList.begin(), clientList.end(), client) == clientList.end())
		return ;
	for(std::vector<Client *>::iterator it = clientList.begin(); it != clientList.end(); it++)
	{
		if (*it == client)
		{
			clientList.erase(it);
			return ;
		}
	}
}

Client	*Channel::isOp(int fd)
{
	std::vector<Client *>::iterator it = clientOperator.begin();
	for(; it != clientOperator.end(); it++)
	{
		if ((*it)->getFd() == fd)
			return (*it);
	}
	return (NULL);
}

void	Channel::addMode(char mode)
{
	if (std::find(modes.begin(), modes.end(), mode) != modes.end())
		return ;
	modes.push_back(mode);
}

void	Channel::removeMode(char mode)
{
	if (std::find(modes.begin(), modes.end(), mode) == modes.end())
		return ;
	for (std::vector<char>::iterator it = modes.begin(); it != modes.end(); it++)
	{
		if (*it == mode)
		{
			modes.erase(it);
			return ;
		}
	}
}

std::vector<char>	Channel::getModes()
{
	return (modes);
}

bool	Channel::checkMode(char mode)
{
	return (std::find(modes.begin(), modes.end(), mode) != modes.end());
}

std::string	Channel::getModeString()
{
	std::string	modeString = "+";

	for(size_t i = 0; i < modes.size(); i++)
		modeString.push_back(modes.at(i));
	if (checkMode('k'))
		modeString.append(" " + _mdp);
	if (checkMode('l'))
		modeString.append(" " + toString(limit));
	return (modeString);
}

std::string	Channel::toString(int i)
{
	std::stringstream ss;
	ss << i;
	return (ss.str());
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

void signalHandler(int signum) 
{
	(void)signum;
	throw std::exception();
}
// void controleC(std::map<std::string, Channel*> chanel_list)
int main(int argc, char **argv)
{
	int server_fd;
	int port;
	int opt = 1;
	struct sockaddr_in address; //represante adresse internet pour ipv4
    int size_addr = sizeof(address);
    char buffer[BUFFER_SIZE];
	Server server;
	std::map<int, Client*> client_map = server.getClientMap();
    std::vector<pollfd> client;
	if (argc < 3)
		return 1;
    assword = argv[2];  
	port = atoi(argv[1]);
	if (port <= 0 || port > 65536)
		return (1);
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

	try 
	{
		while (1)
		{
			server.removeEmptyChans();
			if (poll(&(*client.begin()), client.size(), 0) < 0)
		        	std::cerr << "Error : poll est mourant" << std::endl;
				//iterateur pour voir si evenement en cours
			signal(SIGINT, signalHandler);
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
					Client *new_client = new(Client);
					new_client->setFd(new_socket);
					server.getClientMap().insert(std::pair<int, Client*>(new_socket, new_client));
					client_map[new_socket] = new_client;
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
							std::cout << "WAITING_ROOM = " << client_map[(*it).fd]->waitingRoom << std::endl;
							server.rattrapeReddy(client_map[(*it).fd]->waitingRoom, client_map[(*it).fd]);
							client_map[(*it).fd]->waitingRoom = "";
						}
					}	
					else if (n == 0) //if (n == 0) //deco client
					{
						int fdeco = (*it).fd;
						std::vector<std::string> vecc;
						vecc.push_back("QUIT");
						server.whichCmd(vecc, client_map[(*it).fd]);
						server.removeFromChans(fdeco);
						close(fdeco);
						delete server.getClientMap()[fdeco];
						server.getClientMap().erase(fdeco);
						it = client.erase(it);
					}
					break ;
				}
			}
		}
	}
	catch (const std::exception& e)
	{
		for (std::map<int, Client *>::iterator it = server.getClientMap().begin(); it != server.getClientMap().end(); it++)
			{
				close((*it).second->getFd());
				delete it->second;
			}
		for (std::map<std::string, Channel *>::iterator it = server.getChannelMap().begin(); it != server.getChannelMap().end(); it++)
			delete it->second;
		close(server_fd);
		return (1);
	}
}

Server::Server() //Toute les COMMANDES a gerer = sous forme de CLASS in here arigato gozxaimaumsuiao
{
	this->_cmd.push_back(new FuncCap());
	this->_cmd.push_back(new FuncPass());
	this->_cmd.push_back(new FuncNick());
	this->_cmd.push_back(new FuncUser());
	this->_cmd.push_back(new FuncPing());
	this->_cmd.push_back(new FuncJoin());
	this->_cmd.push_back(new FuncPrivMsg());
	this->_cmd.push_back(new FuncQuit());
	this->_cmd.push_back(new FuncMode());
	this->_cmd.push_back(new FuncTopic());
	this->_cmd.push_back(new FuncInvite());
	this->_cmd.push_back(new FuncKick());
}

Server::~Server()
{
	for (std::list<Acommand*>::iterator it = this->_cmd.begin(); it != this->_cmd.end(); it++)
	{
		delete (*it);
	}
}

std::map<std::string, Channel*>	&Server::getChannelMap()
{
	return (this->chanMap);
}

std::map<int, Client*> &Server::getClientMap()
{
	return (this->clientMap);
}

void	Server::rattrapeReddy(std::string msg, Client *client)
{
	try
	{
		Parser(msg, client);
	}
	catch (std::string msg)
	{
		std::cerr << msg << std::endl;
	}
}

void	Server::Parser(std::string cmd, Client *client) //isole les commandes avec les arguments
{
	int y = 0;
	for (int i = 0; cmd[i]; i++)
	{
		if (cmd[i] == '\n')
		{
			CmdParser(cmd.substr(y, i - y), client); 
			y = i + 1;
		}
	}
}

void	Server::CmdParser(std::string cmd, Client *client)
{
	if (cmd[cmd.size() - 1] == '\r')
		cmd = cmd.erase(cmd.size() - 1);
	std::stringstream ss(cmd);
	std::string word;
	std::vector<std::string> cmdVec;
	while (!ss.eof())
	{
		getline(ss, word, ' ');
		cmdVec.push_back(word); //save la ligne de cmd dans un vec pour easier handling
	}
	whichCmd(cmdVec, client);
}

void	Server::whichCmd(std::vector<std::string> vec, Client *client)
{
	for(std::list<Acommand*>::iterator it = _cmd.begin(); it != _cmd.end(); it++) //on parcours toute la liste de CMD in class Server
	{
		Acommand *cmd = *it;

		if (cmd->getName() == vec[0]) //check if CMD actuel = CMD rentree par client
			cmd->exec(this, client, vec);
	}
}

Channel	*Server::findChannel(std::string name)
{
	if (chanMap.find(name) != chanMap.end())
	{
		Channel *chanPtr = chanMap[name];
		return (chanPtr);
	}
	return (NULL);
}

Client	*Server::findClient(std::string name, int fd)
{
	std::map<int, Client*>::iterator it = clientMap.begin();
	for (; it != clientMap.end(); it++)
	{
		if (fd)
		{
			if (it->second->getFd() == fd)
				return (it->second);
		}
		else
		{
			if (it->second->getNickname() == name)
				return (it->second);
		}
	}
	return (NULL);
}

void	Server::printClients()
{
	std::cout << "PRINTING" << std::endl;
	std::map<int, Client*>::iterator it = clientMap.begin();
	for (; it != clientMap.end(); it++)
		std::cout << "NICK = " << it->second->getNickname() << std::endl;
}

void	Server::removeFromChans(int fd)
{
	Client *client = findClient("null", fd);
	if (!client)
		return ;
	for (std::map<std::string, Channel*>::iterator it = chanMap.begin(); it != chanMap.end(); it++)
	{
		Channel *chanPtr = it->second;

		if (chanPtr->findClient(client->getNickname()))
		{
			chanPtr->removeClientoperator(client);
			chanPtr->removeClient(client);
		}
	}
}

void	Server::removeEmptyChans()
{
	std::map<std::string, Channel*>::iterator currentIt = chanMap.begin();
	std::map<std::string, Channel*>::iterator nextIt;

	while (currentIt != chanMap.end())
	{
		nextIt = currentIt;
		nextIt++;

		Channel *chan = currentIt->second;

		if (chan->nbrClientlist() == 0)
		{
			delete chan;
			chanMap.erase(currentIt++);
		}
		else
			currentIt = nextIt;
	}
}

//////////////// ACOMMAND ////////////////
Acommand::Acommand(std::string name)
{
	_name = name;
}

Acommand::~Acommand()
{
}

std::string	Acommand::getName()
{
	return (_name);
}

//////////////// CAP ////////////////
FuncCap::FuncCap(): Acommand("CAP") //CHAQUE COMMANDE HERITE DU CONSTRUCT ACOMMAND WITH SON NOM
{
}

FuncCap::~FuncCap()
{
}

void	FuncCap::exec(Server *serv, Client *client, std::vector<std::string> vec) const
{
	(void)serv;
	if (vec[1] != "END")
		client->sendMsg(": CAP * LS :", -1);
}

//////////////// PASS ////////////////
FuncPass::FuncPass(): Acommand("PASS")
{

}

FuncPass::~FuncPass()
{
}


void	FuncPass::exec(Server *serv, Client *client, std::vector<std::string> vec) const
{
	(void)serv;
	if (vec.size() < 2)
	{
		client->sendMsg(":" + client->getId() + " 461 " + client->getNickname() + " PASS" + " :Not enough parameters" + "\r", client->getFd()); //NOTENOUGHPARAMS
		return ;
	}
	if (client->getPassBool())
	{
		client->sendMsg(":" + client->getId() + " 462 " + client->getNickname() + " :You may not reregister" + "\r", client->getFd());
		return ;
	}
	if (vec[1] == assword)
	{
		client->boolSetter(0, true);
		client->sendMsg("Password is correct", client->getFd());
		client->tryLogin();
	}
	else
		client->sendMsg(":" + client->getId() + " 464 " + client->getNickname() + " :Password incorrect" + "\r", client->getFd()); //PASSWDMISMATCH
}
//////////////// NICK ////////////////
FuncNick::FuncNick(): Acommand("NICK")
{
}

FuncNick::~FuncNick()
{
}

int		FuncNick::nickParser(std::string nick) const
{
	if (nick[0] == '#' || nick[0] == ':' || nick[0] == '$' || nick.empty())
		return (1);
	for (size_t i = 0; i != nick.length(); i++)
	{
		if (nick[i] == ' ' ||nick[i] == '*' ||nick[i] == '?' ||nick[i] == '!' ||nick[i] == ',' ||nick[i] == '@' ||nick[i] == '.')
			return (1);
	}
	return (0);
}

void	FuncNick::exec(Server *serv, Client *client, std::vector<std::string> vec) const
{
	static int	vd = 0;
	bool alrUsed = false;
	std::map<int, Client*> clientMap = serv->getClientMap();
	std::map<int, Client*>::iterator it = clientMap.begin();
	if (!client->getPassBool()) //check pass first
	{
		client->sendMsg("Enter PASS first", client->getFd());
		return ;
	}
	if (nickParser(vec[1]))
	{
		client->sendMsg(":" + client->getId() + " 432 " + client->getNickname() + " " + vec[1] + " :Erroneus nickname" + "\r", client->getFd());
		return ;
	}
	for (; it != clientMap.end(); it++) //check if nick is already used
	{
		if (vec[1] == it->second->getNickname() && it->first != client->getFd())
			alrUsed = true;
	}
	if (client->isFirstCo()) //if is first conect
	{
		if (alrUsed)
		{ 
			std::stringstream ss;
			ss << vd; // Insert the integer into the stringstream
			std::string str = ss.str();
			vec[1] += str;
			vd++;
		}
		client->stringSetter(0, vec[1]);
		client->boolSetter(1 ,true);
		client->boolSetter(3 ,false); //mark the first connection
		client->tryLogin();
	}
	else //not first co
	{
		if (alrUsed)
			client->sendMsg(":" + client->getId() + " 433 " + client->getNickname() + " " + vec[1] + " :Nickname is already in use" + "\r", client->getFd()); //NICKNAMEINUSE
		else
		{
			client->sendMsg(":" + client->getNickname() + " NICK " + vec[1] + "\r", client->getFd());
			client->stringSetter(0, vec[1]);
			client->boolSetter(1 ,true);
		}
	}
}

//////////////// USER ////////////////
FuncUser::FuncUser(): Acommand ("USER")
{
}

FuncUser::~FuncUser()
{
}

void	FuncUser::exec(Server *serv, Client *client, std::vector<std::string> vec) const
{
	(void)serv;
	if (!client->getPassBool()) //check pass first
	{
		client->sendMsg("Enter PASS first", client->getFd());
		return ;
	}
	if (vec.size() < 5)
	{
		client->sendMsg(":" + client->getId() + " 461 " + client->getNickname() + " JOIN" + " :Not enough parameters" + "\r", client->getFd()); //NOTENOUGHPARAMS
		return ;
	}
	std::vector<std::string>::iterator it = vec.begin() + 5;
	vec[4].erase(0, 1);
	for(; it != vec.end(); it++)
	{
		vec[4] += " ";
		vec[4] += *it;
	}

	client->stringSetter(1, vec[1]);
	client->stringSetter(2, vec[2]);
	client->stringSetter(3, vec[3]);
	client->stringSetter(4, vec[4]);
	client->boolSetter(2, true);
	client->tryLogin();
}

//////////////// PING ////////////////
FuncPing::FuncPing() : Acommand("PING")
{
}

FuncPing::~FuncPing()
{
}

void	FuncPing::exec(Server *serv, Client *client, std::vector<std::string> vec) const
{
	(void)serv;
	(void)vec;
	client->sendMsg(":server PONG :" + client->getNickname(), -1);
}

//////////////// JOIN ////////////////
FuncJoin::FuncJoin() : Acommand("JOIN")
{
}

FuncJoin::~FuncJoin()
{
}

std::string	FuncJoin::stringOfUsers(Channel *chan) const
{
	std::string ret;
	std::vector<Client *>::iterator it = chan->getClientlist().begin();
	std::vector<Client *> tmp = chan->getClientoperator();
	while(it != chan->getClientlist().end())
	 {
		std::vector<Client *>::iterator it2 = tmp.begin();
		for (; it2 != tmp.end(); it2++)
			if ((*it2)->getNickname() == (*it)->getNickname())
				break ;
		if (it2 != tmp.end())
			ret = ret + "@" + (*it)->getNickname();
		else
			ret += (*it)->getNickname();
		it++;
		if (it != chan->getClientlist().end())
			ret += ' ';
	 }
	 return (ret);
}

void	FuncJoin::exec(Server *serv, Client *client, std::vector<std::string> vec) const
{
	if (!client->isLogged())
	{
		client->sendMsg("You must login first, using PASS USER NICK functions.", client->getFd());
		return ;
	}
	if (vec.size() < 2)
	{
			client->sendMsg(":" + client->getId() + " 461 " + client->getNickname() + " JOIN" + " :Not enough parameters" + "\r", client->getFd()); //NOTENOUGHPARAMS
			return ;
	}

	if (vec[1][0] != '#')
	{
		client->sendMsg(":" + client->getId() + " 476 " + client->getNickname() + " " + vec[1] + " :Incorrect format" + "\r", client->getFd()); //BADCHANMASK
		return ;
	}
	
	Channel *chanPtr = serv->findChannel(vec[1]);
	std::string password;

	if (vec.size() == 3)
		password = vec[2];
	else
		password = "";

	if (!chanPtr) //if chan does not exist, create it
	{

		chanPtr = new Channel();
		chanPtr->setShortname(vec[1]);
		chanPtr->addClientcreator(client);
		chanPtr->addClientoperator(client);
		if (password != "")
		{
			chanPtr->addMode('k');
			chanPtr->setMdp(password);
		}
		serv->getChannelMap().insert(std::pair<std::string, Channel*>(vec[1], chanPtr)); //created and added to the chanmap
	}
	else
	{
		if (chanPtr->findClient(client->getNickname()))
		{
			client->sendMsg("ERROR: You are already on that channel.", client->getFd());
			return ;
		}
		if (chanPtr->checkMode('k') && password != chanPtr->getMdp()) //password
		{
			client->sendMsg(":" + client->getId() + " 475 " + client->getNickname() + " " + chanPtr->getShortname() + " :Cannot join channel (+k)" + "\r", client->getFd());
			return ;
		}
		if (chanPtr->checkMode('l')) //limit
		{
			if (chanPtr->nbrClientlist() >= chanPtr->getLimit())
			{
				client->sendMsg(":" + client->getId() + " 471 " + client->getNickname() + " " + chanPtr->getShortname() + " :Cannot join channel (+l)" + "\r", client->getFd());
				return ;
			}
		}
		if (chanPtr->checkMode('i') && !chanPtr->isInvited(client)) //invite only
		{
				client->sendMsg(client->getId() + " 473 " + client->getNickname() + " " + chanPtr->getShortname() + " :Cannot join channel (+i)" + "\r", client->getFd());
				return;
		}
	}
	chanPtr->addClientlist(client); //add client to the clientlist)
	std::string userList = stringOfUsers(chanPtr);
	chanPtr->msgChannel(client->getFd(), ":" + client->getNickname() + " JOIN :" + vec[1] + "\r", 0); //JOIN
	if (!chanPtr->getTopic().empty())
	{
		client->sendMsg(":" + client->getId() + " 332 " + client->getNickname() + " " + chanPtr->getShortname() + " :" + chanPtr->getTopic() + "\r", client->getFd()); //RPL TOPIC
	}
	client->sendMsg(":" + client->getId() + " 353 " + client->getNickname() + " " + "=" + " " + chanPtr->getShortname() + " :" + userList + "\r", client->getFd()); //NAMREPLY
	client->sendMsg(":" + client->getId() + " 366 " + client->getNickname() + " " + chanPtr->getShortname() + " :End of /NAMES list" + "\r", client->getFd()); //ENDOFNAMES
}

//////////////// PRIVMSG ////////////////
FuncPrivMsg::FuncPrivMsg(): Acommand("PRIVMSG")
{
}

FuncPrivMsg::~FuncPrivMsg()
{
}

std::string	FuncPrivMsg::createMsg(std::vector<std::string> vec) const
{
	std::string ret;
	std::vector<std::string>::iterator it = vec.begin() + 2;
	for (; it != vec.end(); it++)
	{
		ret += (*it);
		if (it != vec.end())
			ret += " ";
	}
	return (ret);
}

void	FuncPrivMsg::exec(Server *serv, Client *client, std::vector<std::string> vec) const
{
	Channel	*chanPtr;
	Client	*target;
	if (!client->isLogged())
	{
		client->sendMsg("You must login first, using PASS USER NICK functions.", client->getFd());
		return ;
	}
	if (vec.size() < 3)
	{
		client->sendMsg("Cannot send empty message. Try again.", client->getFd());
		return ;
	}
	if (vec[2][0] == ':')
		vec[2].erase(0,1); //erase le ':'
	std::string msg = createMsg(vec); 
	if (vec[1].find('#') != std::string::npos) //msging in channel
	{
		chanPtr = serv->findChannel(vec[1]);
		if (!chanPtr)
		{
			client->sendMsg(":" + client->getId() + " 401 " + client->getNickname() + " " + vec[1] + " :No such channel" + "\r", client->getFd()); //NOSUCHCHANNEL
			return ;
		}
		target = chanPtr->findClient(client->getNickname()); //here target is sender
		if (!target) //checking if sender is in channel
		{
			client->sendMsg(":" + client->getId() + " 442 " + client->getNickname() + " " + vec[1] + " :You're not on that channel" + "\r", client->getFd()); 
			return ;
		}
		chanPtr->msgChannel(target->getFd(), ":" + client->getNickname() + " PRIVMSG " + vec[1] + " :" + msg, 1);
	}
	else
	{
		target = serv->findClient(vec[1], 0); //here target is receiver
		if (!target) 
		{
			client->sendMsg(":" + client->getId() + " 401 " + client->getNickname() + " " + vec[1] + " :No such nick" + "\r", client->getFd()); //NOSUCHNICK
			return ;
		}
		client->sendMsg(":" + client->getNickname() + " PRIVMSG " + target->getNickname() + " :" + msg, target->getFd());
	}
}

////////////// QUIT /////////////////
FuncQuit::FuncQuit(): Acommand("QUIT")
{
}

FuncQuit::~FuncQuit()
{
}

void	FuncQuit::exec(Server *serv, Client *client, std::vector<std::string> vec) const
{
	if (serv->getChannelMap().empty())
		return ;

	std::string reason;
	for(size_t i = 1; i < vec.size(); i++)
	{
		if (!reason.empty())
			reason.append(" ");
		reason.append(vec[i]);
	}
	std::string fullRep = ":" + client->getNickname() + " QUIT " + (reason.empty() ? ":Client Quit" : reason) + "\r";

	std::map<std::string, Channel*>::iterator it = serv->getChannelMap().begin();
	for(; it != serv->getChannelMap().end(); it++) //iter throu Channels
	{
		Channel *tmp = it->second;
		std::vector<Client *>::iterator it2 = tmp->getClientlist().begin(); //iter throu Clients inside Channel
		for (; it2 != tmp->getClientlist().end(); it2++)
		{
			if ((*it2)->getFd() == client->getFd())
				tmp->msgChannel(client->getFd(), fullRep, 0);
		}
	}
	client->resetLogin();
}

////////////// MODE /////////////////
FuncMode::FuncMode(): Acommand("MODE")
{
}

FuncMode::~FuncMode()
{
}

bool	FuncMode::addMode(Channel *chan, std::vector<std::string> vec, Client *client) const
{
	std::string modes = vec[2];
	if (modes.at(0) == '+')
		modes.erase(0,1);

	if (modes.at(0) == 'i') //set ivntie
	{
		if (chan->checkMode('i'))
			return (false);
		chan->addMode('i');
	}

	if (modes.at(0) == 't') //set tpico
	{
		if (chan->checkMode('t'))
			return (false);
		chan->addMode('t');
	}

	if (modes.at(0) == 'k') //add pasowrds
	{
		if (vec.size() < 4)
		{
			client->sendMsg(":" + client->getId() + " 461 " + client->getNickname() + " MODE" + " :Not enough parameters" + "\r", client->getFd()); //NOTENOUGHPARAMS
			return (false);
		}
		if (chan->checkMode('k'))
			return (false);
		chan->addMode('k');
		chan->setMdp(vec[3]);
	}

	if (modes.at(0) == 'l') //add lm,it
	{
		if (vec.size() < 4)
		{
			client->sendMsg(":" + client->getId() + " 461 " + client->getNickname() + " MODE" + " :Not enough parameters" + "\r", client->getFd()); //NOTENOUGHPARAMS
			return (false);
		}
		int	limit = ft_stoi(vec[3]);
		if (!chan->checkMode('l'))
			chan->addMode('l');
		chan->setLimit(limit);
	}

	if (modes.at(0) == 'o') //add orpatoree
	{
		if (vec.size() < 4)
		{
			client->sendMsg(":" + client->getId() + " 461 " + client->getNickname() + " MODE" + " :Not enough parameters" + "\r", client->getFd()); //NOTENOUGHPARAMS
			return (false);
		}

		Client *clientPtr = chan->findClient(vec[3]);

		if(!clientPtr)
		{
			client->sendMsg(":" + client->getId() + " 401 " + client->getNickname() + " " + vec[3] + " :No such nick" + "\r", client->getFd()); //NOSUCHNICK
			return (false);
		}
		if (chan->isOp(clientPtr->getFd()))
			return (false);
		chan->addClientoperator(clientPtr);
		chan->msgChannel(client->getFd(), ":" + client->getId() + " MODE " + chan->getShortname() + " " + "+o " + clientPtr->getNickname() + "\r", 0); //CHANNELMODEIS
		return (false);
	}
	return (true);
}

bool	FuncMode::removeMode(Channel *chan, std::vector<std::string> vec, Client *client) const
{
	std::string modes = vec[2];
	if (modes.at(0) != '-') //check that it actually starts with -
		return (false);
	modes.erase(0, 1);

	if (modes.at(0) == 'i') //remove invte
	{
		if (!chan->checkMode('i'))
			return (false);
		chan->removeMode('i');
	}

	if (modes.at(0) == 't') //topoic restrict
	{
		if (!chan->checkMode('t'))
			return (false);
		chan->removeMode('t');
	}

	if (modes.at(0) == 'k') //remove passwofr
	{
		if (!chan->checkMode('k'))
			return (false);
		chan->removeMode('k');
		chan->deleteMdp();
	}

	if (modes.at(0) == 'l') //remove limti
	{
		if (!chan->checkMode('l'))
			return (false);
		chan->removeMode('l');
	}

	if (modes.at(0) == 'o') //revoke opeartor
	{
		if (vec.size() < 4) //check if victim names are given
		{
			client->sendMsg(":" + client->getId() + " 461 " + client->getNickname() + " MODE" + " :Not enough parameters" + "\r", client->getFd()); //NOTENOUGHPARAMS
			return (false);
		}

		Client *clientPtr = chan->findClient(vec[3]);
		if(!clientPtr)
		{
			client->sendMsg(":" + client->getId() + " 401 " + client->getNickname() + " " + vec[3] + " :No such nick" + "\r", client->getFd()); //NOSUCHNICK
			return (false);
		}
		if (!chan->isOp(clientPtr->getFd()))
			return (false);
		chan->removeClientoperator(clientPtr);
		chan->msgChannel(client->getFd(), ":" + client->getId() + " MODE " + chan->getShortname() + " " + "-o " + clientPtr->getNickname() + "\r", 0); //CHANNELMODEIS
		return (false);
	}

	return (true);
}

void	FuncMode::exec(Server *serv, Client *client, std::vector<std::string> vec) const
{
	if (!client->isLogged())
	{
		client->sendMsg("You must login first, using PASS USER NICK functions.", client->getFd());
		return ;
	}
	if (vec.size() < 2)
		client->sendMsg(":" + client->getId() + " 461 " + client->getNickname() + " MODE" + " :Not enough parameters" + "\r", client->getFd()); //NOTENOUGHPARAMS
	if (vec[1].at(0) != '#' && vec[1].compare(client->getNickname()) == 0) //if mode with self nick is used
	{
		client->sendMsg(":" + client->getId() + " 221 " + client->getNickname() + " " + "+wi" + "\r", client->getFd()); //UMODEIS
		return ;
	}

	Channel *chanPtr = serv->findChannel(vec[1]);

	if (!chanPtr) //if channel not exist
	{
		client->sendMsg(":" + client->getId() + " 403 " + client->getNickname() + " " + vec[1] + " :No such channel" + "\r", client->getFd()); //NOSUCHCHANNEL
		return ;	
	}
	if (!chanPtr->findClient(client->getNickname())) //if client not in channel
	{
		client->sendMsg(":" + client->getId() + " 442 " + client->getNickname() + " " + chanPtr->getShortname() + " :You're not on that channel" + "\r", client->getFd());
		return ;
	}
	if (vec.size() == 2) //if without modestring
	{
		client->sendMsg(":" + client->getId() + " 324 " + client->getNickname() + " " + chanPtr->getShortname() + " " + chanPtr->getModeString() + "\r", client->getFd()); //CHANNELMODEIS
		return ;
	}
	if (!chanPtr->isOp(client->getFd())) //if client not OP of channel
	{
		client->sendMsg(":" + client->getId() + " 482 " + client->getNickname() + " " + chanPtr->getShortname() + " :You're not channel operator" + "\r", client->getFd());
		return ;
	}
	std::string	modes = vec[2];
	std::string	availableModes = "itkol+-";
	bool		updatedModes = false;

	if (availableModes.find(modes[1]) == std::string::npos) //check if requested mode is handled
	{
		client->sendMsg(":" + client->getId() + " 472 " + client->getNickname() + " " + serv->toString(modes[1]) + " :is unknown mode char to me" + "\r", client->getFd()); //UNKNOWN MODE
		return ;
	}

	if (modes.at(0) == '-' && modes.size() > 1)
		updatedModes = this->removeMode(chanPtr, vec, client);
	else if (modes.size() > 1)
		updatedModes = this->addMode(chanPtr, vec, client);
	if (updatedModes)
		chanPtr->msgChannel(client->getFd(), ":" + client->getId() + " 324 " + client->getNickname() + " " + chanPtr->getShortname() + " " + chanPtr->getModeString() + "\r", 0); //CHANNELMODEIS
}

/////////////////// TOPIC ////////////////////
FuncTopic::FuncTopic(): Acommand("TOPIC")
{
}

FuncTopic::~FuncTopic()
{
}

void	FuncTopic::exec(Server *serv, Client *client, std::vector<std::string> vec) const
{
	if (!client->isLogged())
	{
		client->sendMsg("You must login first, using PASS USER NICK functions.", client->getFd());
		return ;
	}
	if (vec.size() < 2)
	{
		client->sendMsg(":" + client->getId() + " 461 " + client->getNickname() + " TOPIC" + " :Not enough parameters" + "\r", client->getFd()); //NOTENOUGHPARAMS
		return ;
	}

	Channel *chanPtr = serv->findChannel(vec[1]);

	if (!chanPtr || vec[2].empty())
	{
		client->sendMsg(":" + client->getId() + " 403 " + client->getNickname() + " " + vec[1] + " :No such channel" + "\r", client->getFd()); //NOSUCHCHANNEL
		return ;
	}
	if (vec.size() == 2)
	{
		if (chanPtr->getTopic().empty())
			client->sendMsg(":" + client->getId() + " 331 " + client->getNickname() + " " + chanPtr->getShortname() + " :No topic is set." + "\r", client->getFd()); //NOTOPIC
		else
			client->sendMsg(":" + client->getId() + " 332 " + client->getNickname() + " " + chanPtr->getShortname() + " :" + chanPtr->getTopic() + "\r", client->getFd()); //TOPIC
		return ;
	}

	if (!chanPtr->findClient(client->getNickname())) //check if client in chan
	{
		client->sendMsg(":" + client->getId() + " 442 " + client->getNickname() + " " + chanPtr->getShortname() + " :You're not on that channel" + "\r", client->getFd()); //NOTONCHANNEL
		return ;
	}
	if (!chanPtr->checkMode('t') && !chanPtr->isOp(client->getFd())) //check if is OP
	{
		client->sendMsg(":" + client->getId() + " 482 " + client->getNickname() + " " + chanPtr->getShortname() + " :You're not channel operator" + "\r", client->getFd()); //CHANOPRIVSNEEDED
		return ;
	}

	std::string topic;
	for (size_t i = 2; i < vec.size(); i++)
	{
		if (!topic.empty())
			topic.append(" ");
		topic.append(vec[i]);
	}

	chanPtr->setTopic(topic[0] == ':' ? topic.erase(0, 1) : topic);
	chanPtr->msgChannel(client->getFd(), ":" + client->getId() + " TOPIC " + chanPtr->getShortname() + " " + topic + "\r", 0);
}

/////////// INVITE //////////////
FuncInvite::FuncInvite(): Acommand("INVITE")
{
}

FuncInvite::~FuncInvite()
{
}

void	FuncInvite::exec(Server *serv, Client *client, std::vector<std::string> vec) const
{
	if (!client->isLogged())
	{
		client->sendMsg("You must login first, using PASS USER NICK functions.", client->getFd());
		return ;
	}
	if (vec.size() < 3)
	{
		client->sendMsg(":" + client->getId() + " 461 " + client->getNickname() + " INVITE" + " :Not enough parameters" + "\r", client->getFd()); //NOTENOUGHPARAMS
		return ;
	}

	Channel *chanPtr = serv->findChannel(vec[2]);
	Client	*target = serv->findClient(vec[1], 0);

	if (!chanPtr)
	{
		client->sendMsg(":" + client->getId() + " 403 " + client->getNickname() + " " + vec[2] + " :No such channel" + "\r", client->getFd()); //NOSUCHCHANNEL
		return ;
	}
	if (!target)
	{
		client->sendMsg(":" + client->getId() + " 401 " + client->getNickname() + " " + vec[1] + " :No such nick" + "\r", client->getFd()); //NOSUCHNICK
		return ;
	}
	if (!chanPtr->findClient(client->getNickname())) //if inviter is no part of channel
	{
		client->sendMsg(":" + client->getId() + " 442 " + client->getNickname() + " " + chanPtr->getShortname() + " :You're not on that channel" + "\r", client->getFd()); //NOTONCHANNEL
		return ;
	}
	if (chanPtr->checkMode('i') && !chanPtr->isOp(client->getFd())) //if client is not OP
	{	
		client->sendMsg(":" + client->getId() + " 482 " + client->getNickname() + " " + chanPtr->getShortname() + " :You're not channel operator" + "\r", client->getFd()); //CHANOPRIVSNEEDED
	}
	if (chanPtr->findClient(vec[1])) //if invitee is already in channel
	{
		client->sendMsg(":" + client->getId() + " 443 " + client->getNickname() + " " + chanPtr->getShortname() + " :is already on channel" + "\r", client->getFd()); //USERONCHANNEL
		return ;
	}

	target->sendMsg(":" + client->getId() + " INVITE " + target->getNickname() + " " + chanPtr->getShortname() + "\r", target->getFd());
	client->sendMsg(":" + client->getId() + " 341 " + client->getNickname() + " " + chanPtr->getShortname() + " " + target->getNickname() + "\r", client->getFd()); //INVITING
	if (!chanPtr->isInvited(target))
		chanPtr->addInvite(target);
}

//////////////// KICK ///////////////////
FuncKick::FuncKick(): Acommand("KICK")
{
}

FuncKick::~FuncKick()
{
}

std::string	FuncKick::createMsg(std::vector<std::string> vec) const
{
	std::string ret;
	std::vector<std::string>::iterator it = vec.begin() + 3;
	for (; it != vec.end(); it++)
	{
		ret += (*it);
		if (it != vec.end() - 1)
			ret += " ";
	}
	return (ret);
}

void	FuncKick::exec(Server *serv, Client *client, std::vector<std::string> vec) const
{
	if (vec.size() < 4)
	{
		client->sendMsg(":" + client->getId() + " 461 " + client->getNickname() + " KICK" + " :Not enough parameters" + "\r", client->getFd()); //NOTENOUGHPARAMS
		return ;
	}
	Channel *chanPtr = serv->findChannel(vec[1]);

	if (!chanPtr)
	{
		client->sendMsg(":" + client->getId() + " 403 " + client->getNickname() + " " + vec[1] + " :No such channel" + "\r", client->getFd()); //NOSUCHCHANNEL
		return ;
	}
	if (!chanPtr->isOp(client->getFd()))
	{
		client->sendMsg(":" + client->getId() + " 482 " + client->getNickname() + " " + chanPtr->getShortname() + " :You're not channel operator" + "\r", client->getFd()); //CHANOPRIVSNEEDED
		return ;
	}

	Client *clientPtr = serv->findClient(vec[2], 0);
	if (!clientPtr)
	{
		client->sendMsg(":" + client->getId() + " 401 " + client->getNickname() + " " + vec[2] + " :No such nick" + "\r", client->getFd()); //NOSUCHNICK
		return ;
	}
	if (!chanPtr->findClient(vec[2]))
	{
		client->sendMsg(":" + client->getId() + " 441 " + client->getNickname() + " " + chanPtr->getShortname() + " " + vec[2] + " :They aren't on that channel" + "\r", client->getFd()); //USERNOTINCHANNEL
		return ;
	}

	std::string	msg = createMsg(vec);

	chanPtr->msgChannel(client->getFd(), ":" + client->getId() + " KICK " + chanPtr->getShortname() + " " + clientPtr->getNickname() + " :" + msg + "\r", 0); //KICKING

	chanPtr->removeClient(clientPtr);
	chanPtr->removeClientoperator(clientPtr);
}
