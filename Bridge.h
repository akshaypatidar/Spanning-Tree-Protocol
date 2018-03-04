#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <map>
#include <sstream>
using namespace std;
struct LanSegment{
	string name;
	vector<pair<string, bool> > bridges;
	//vector<string> hosts;
	//string DesignatedBridge;
};

struct Message{
	string RootID;
	int Distance;
	string Sender;
	void print(){
		cout<<"("<<RootID<<","<<Distance<<","<<Sender<<")"<<endl;
	}
};
struct Packet{
	int id;
	int time;
	string source;
	string destination;
	string lan;
	Packet(int,int,string,string,string);
};
struct Received{
	int id;
	Message m;
	int time;
	string lan;
	Received(int,Message,int,string);
};
class Bridge{
	public :
	string ID;
	vector<pair<string,bool> > Lan_Segments;
	vector<bool> DestinationPort;
	Message ConfigMessage;
	string RootPort;
	map<string,string> ForwardingTable;
	
	Bridge();
	void PopulateBridge(string,vector<pair<string,bool> >);
	void ReceiveConfigMessage(Message,int,string,Bridge[],queue<Received>&,bool,map<string,LanSegment>&);
	void SendConfigMessage(Bridge[],queue<Received>&,bool,map<string,LanSegment>&);
	void ForwardMessage(string,string,string,Bridge[],map<string,LanSegment>&,queue<Packet>&,int);


};