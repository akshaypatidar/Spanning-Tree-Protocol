#include "Bridge.h"
Received::Received(int ID,Message mess,int t,string LAN){
	id = ID;
	m = mess;
	time = t;
	lan = LAN;
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
		for(int i=0;i<Lan_Segments.size();i++){
			if(lans[Lan_Segments[i].first].name==lan){
				//cout<<Lan_Segments[i].first.name<<endl;
				DestinationPort[i]=false;
			}
		}
			
	}
	else if(m.RootID == ConfigMessage.RootID && m.Distance == ConfigMessage.Distance && stoi(m.Sender.substr(1)) < stoi(ID.substr(1))){
		for(int i=0;i<Lan_Segments.size();i++){
			if(lans[Lan_Segments[i].first].name==lan){
				//cout<<Lan_Segments[i].first.name<<endl;
				DestinationPort[i]=false;
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
			if(lans[Lan_Segments[i].first].name==lan){
				DestinationPort[i]=false;
			}
			else if(Lan_Segments[i].second){
				for(int j=0; j<lans[Lan_Segments[i].first].bridges.size();j++){
					if(lans[Lan_Segments[i].first].bridges[j].first != ID){
						//cout<<time<<" "<<Lan_Segments[i].first.bridges[j]<<endl;
						//b[stoi(Lan_Segments[i].first.bridges[j].substr(1))].ReceiveConfigMessage(tosend,time+1,Lan_Segments[i].first.name,b);
						Received received(stoi(lans[Lan_Segments[i].first].bridges[j].first.substr(1)),tosend,time+1,lans[Lan_Segments[i].first].name);
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
					
					Received received(stoi(lans[Lan_Segments[i].first].bridges[j].first.substr(1)),tosend,1,lans[Lan_Segments[i].first].name);
					q.push(received);
				}
			}
		}
	}
}
	
void Bridge::ForwardMessage(string source, string destination,map<string,string> hostmap,string lan){
	if(ForwardingTable.count(source) == 0){
		ForwardingTable.insert(make_pair(source,lan));
	}
	if(ForwardingTable.count(destination) > 0){
		//ForwardingTable[destination]
	}
}
