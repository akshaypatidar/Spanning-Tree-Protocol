#include "Bridge.h"

int main(){
	int trace;
	int numBridges;
	cin>>trace;
	cin>>numBridges;
	Bridge bridge[numBridges+1];
	//vector<string> LANSname[numBridges+1];
	map<string, LanSegment> lans;
	

	for(int i=0;i<numBridges+1;i++){
		//int n;cin>>n;
		string line,name;
		getline(cin,line);
		istringstream iss(line);
		vector<pair<string,bool> > v;
		int count=0;
		while(iss >> name){
			
			//LANSname[i].push_back(name);
			if(count != 0){
				v.push_back(make_pair(name,true));
				if(lans.count(name)>0){
					lans[name].bridges.push_back(make_pair("B"+to_string(i),true));
				}
				else{
					LanSegment l;
					l.name = name;
					l.bridges.push_back(make_pair("B"+to_string(i),true));
					lans.insert(pair<string,LanSegment>(name,l));
				}
			}
			count++;
		}
		bridge[i].PopulateBridge("B" + to_string(i),v);

	}

	
	queue<Received> q;
	for(int i=1;i<numBridges+1;i++)
		bridge[i].SendConfigMessage(bridge,q,trace,lans);

	
	while(!q.empty()){
		Received temp = q.front();
		//temp.m.print();
		bridge[temp.id].ReceiveConfigMessage(temp.m,temp.time,temp.lan,bridge,q,trace,lans);
		q.pop();


	}
	
	
	//cout<<lans.size()<<endl;

	map<string,string> hosts;
	for(int i=0;i<lans.size();i++){
		string line,h,lan;
		getline(cin,line); 
		istringstream iss(line);
		int count=0;
		while(iss >> h){
			if(count==0)
				lan=h[0];
			else{				
				hosts.insert(make_pair(h,lan));
			}			
			count++;
		}
	}
	

	int T;cin>>T;
	for(int i=0;i<T;i++){
		if(i==0){
			for(int i=1;i<numBridges+1;i++){
				cout<<bridge[i].ID<<": ";
				for(int j=0;j<bridge[i].Lan_Segments.size();j++){
					if(bridge[i].DestinationPort[j]){
						cout<<lans[bridge[i].Lan_Segments[j].first].name<<"-DP"<<" ";
					}
					else if(bridge[i].RootPort==lans[bridge[i].Lan_Segments[j].first].name){
						cout<<lans[bridge[i].Lan_Segments[j].first].name<<"-RP"<<" ";
					}
					else{
						cout<<lans[bridge[i].Lan_Segments[j].first].name<<"-NP"<<" ";				
					}

				}
			cout<<endl;
	}
		}
		string h1,h2;
		cin>>h1>>h2;
		queue<Packet> forwardingqueue;
		for(int i=0;i<lans[hosts[h1]].bridges.size();i++){
			if(lans[hosts[h1]].bridges[i].second){
				Packet p(stoi(lans[hosts[h1]].bridges[i].first.substr(1)),0,h1,h2,hosts[h1]);			
				forwardingqueue.push(p);
			}		
		}
		while(!forwardingqueue.empty()){
			Packet temp = forwardingqueue.front();
			bridge[temp.id].ForwardMessage(temp.source,temp.destination,temp.lan,bridge,lans,forwardingqueue,temp.time);
			forwardingqueue.pop();
		}
		for(int i=1;i<numBridges+1;i++){
			cout<<"B"<<i<<":"<<endl;
			cout<<"HOST ID | FORWARDING PORT"<<endl;
			map<string,string>::iterator it;
			for(it=bridge[i].ForwardingTable.begin();it!=bridge[i].ForwardingTable.end();it++){
				cout<<it->first<<" | "<<it->second<<endl;
			}
		}
		cout<<endl;
	}

	
	//bridge[5].ForwardMessage("H9","H2","D",bridge,lans);
	
	//cout<<lans["B"].bridges[1]<<endl;
	//cout<<LANSname[1][1]<<endl;
	//cout<<bridge[1].Lan_Segments[0].first.bridges[1]<<endl;
	
	return 0;
}