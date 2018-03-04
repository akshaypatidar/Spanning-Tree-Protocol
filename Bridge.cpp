#include "Bridge.h"
Received::Received(int ID,Message mess,int t,string LAN){
	id = ID;
	m = mess;
	time = t;
	lan = LAN;
}
Packet::Packet(int ID,int t,string s,string d, string LAN){
	id=ID;
	time=t;
	source=s;
	destination=d;
	lan =LAN;
}

Bridge::Bridge(){
	
}
void Bridge::PopulateBridge(string id,vector<pair<string,bool> > lans){
	ID = id;
	Lan_Segments = lans;
	ConfigMessage.RootID = id;
	ConfigMessage.Distance = 0;
	ConfigMessage.Sender = id;
	for(int i=0;i<lans.size();i++){
		DestinationPort.push_back(true);
	}
}

void Bridge::ReceiveConfigMessage(Message m,int time, string lan,Bridge b[],queue<Received>& q,bool trace,map<string,LanSegment> &lans){
	bool resend = false;
	bool makefalse = false;
	if(trace){
		cout<<time<<" "<<ID<<" "<<"r"<<" ";
		m.print();
	}

	if(stoi(m.RootID.substr(1)) < stoi(ConfigMessage.RootID.substr(1))){
		ConfigMessage.RootID = m.RootID;
		RootPort = lan;
		ConfigMessage.Distance = m.Distance+1;
		ConfigMessage.Sender = m.Sender;
		resend = true;
		
	}
	else if(m.RootID == ConfigMessage.RootID && m.Distance < ConfigMessage.Distance-1){
		ConfigMessage.Distance = m.Distance+1;
		RootPort = lan;
		ConfigMessage.Sender = m.Sender;
		resend = true;		
	}
	else if(m.RootID == ConfigMessage.RootID && m.Distance == ConfigMessage.Distance - 1 && stoi(m.Sender.substr(1)) < stoi(ConfigMessage.Sender.substr(1))){
		ConfigMessage = m;
		RootPort=lan;
		resend = true;
		
	}
	else if(m.RootID == ConfigMessage.RootID && m.Distance == ConfigMessage.Distance - 1 && stoi(m.Sender.substr(1)) > stoi(ConfigMessage.Sender.substr(1))){
		makefalse=true;
			
	}
	else if(m.RootID == ConfigMessage.RootID && m.Distance == ConfigMessage.Distance && stoi(m.Sender.substr(1)) < stoi(ID.substr(1))){
		makefalse=true;
	}
	else if(m.RootID == ConfigMessage.RootID && m.Distance == ConfigMessage.Distance - 1 && m.Sender == ConfigMessage.Sender){
		makefalse=true;
	}
	if(makefalse){
		for(int i=0;i<Lan_Segments.size();i++){
			if(Lan_Segments[i].first==lan){
				//cout<<Lan_Segments[i].first.name<<endl;
				DestinationPort[i]=false;
				if(lan != RootPort)
					Lan_Segments[i].second=false;
				
			}
		}
		for(int i=0;i<lans[lan].bridges.size();i++){
			if(lans[lan].bridges[i].first == ID && lan != RootPort){
				lans[lan].bridges[i].second = false;
				
			}
		}
	}

	Message tosend = ConfigMessage;
	tosend.Sender = ID;
	if(trace){
		cout<<time<<" "<<ID<<" "<<"s"<<" ";
		tosend.print();
	}
	if(resend){
	
		for(int i=0;i<Lan_Segments.size();i++){
			if(Lan_Segments[i].first==lan){
				DestinationPort[i]=false;
			}
			else if(Lan_Segments[i].second){
				for(int j=0; j<lans[Lan_Segments[i].first].bridges.size();j++){
					if(lans[Lan_Segments[i].first].bridges[j].first != ID){
						//cout<<time<<" "<<Lan_Segments[i].first.bridges[j]<<endl;
						//b[stoi(Lan_Segments[i].first.bridges[j].substr(1))].ReceiveConfigMessage(tosend,time+1,Lan_Segments[i].first.name,b);
						Received received(stoi(lans[Lan_Segments[i].first].bridges[j].first.substr(1)),tosend,time+1,Lan_Segments[i].first);
						q.push(received);

					}
				}
			}
		}
	}
}

void Bridge::SendConfigMessage(Bridge b[],queue<Received>& q,bool trace,map<string,LanSegment> &lans){

	Message tosend = ConfigMessage;
	tosend.Sender = ID;
	if(trace){
		cout<<0<<" "<<ID<<" "<<"s"<<" ";
		tosend.print();
	}
	for(int i=0;i<Lan_Segments.size();i++){
		if(Lan_Segments[i].second){
			for(int j=0; j<lans[Lan_Segments[i].first].bridges.size();j++){
				if(lans[Lan_Segments[i].first].bridges[j].first != ID){
					//cout<<Lan_Segments[i].first.name<<endl;
					//b[stoi(Lan_Segments[i].first.bridges[j].substr(1))].ReceiveConfigMessage(tosend,1,Lan_Segments[i].first.name,b);
					
					Received received(stoi(lans[Lan_Segments[i].first].bridges[j].first.substr(1)),tosend,1,Lan_Segments[i].first);
					q.push(received);
				}
			}
		}
	}
}
	
void Bridge::ForwardMessage(string source, string destination,string lan,Bridge b[],map<string,LanSegment> &lans,queue<Packet> &que,int time){
	if(ForwardingTable.count(source) == 0){
		ForwardingTable.insert(make_pair(source,lan));
	}
	if(ForwardingTable.count(destination) > 0){
		for(int i=0;i<lans[ForwardingTable[destination]].bridges.size();i++){
			if(lans[ForwardingTable[destination]].bridges[i].second && lans[ForwardingTable[destination]].bridges[i].first != ID){
				//cout<<stoi(lans[ForwardingTable[destination]].bridges[i].first.substr(1))<<endl;
				Packet packet(stoi(lans[ForwardingTable[destination]].bridges[i].first.substr(1)),time,source,destination,ForwardingTable[destination]);
				que.push(packet);
			}
		}
	}
	else{
		for(int j=0;j<Lan_Segments.size();j++){
			if(Lan_Segments[j].first != lan && Lan_Segments[j].second){
				//cout <<Lan_Segments[j].first<<endl;

				for(int i=0;i<lans[Lan_Segments[j].first].bridges.size();i++){
					if(lans[Lan_Segments[j].first].bridges[i].second && lans[Lan_Segments[j].first].bridges[i].first != ID){
						//cout<<stoi(lans[Lan_Segments[j].first].bridges[i].first.substr(1))<<endl;
						//b[stoi(lans[Lan_Segments[j].first].bridges[i].first.substr(1))].ForwardMessage(source,destination,Lan_Segments[j].first,b,lans);
						Packet packet(stoi(lans[Lan_Segments[j].first].bridges[i].first.substr(1)),time+1,source,destination,Lan_Segments[j].first);
						que.push(packet);
					}
				}
			}
		}

	}
}
