#include "Bridge.h"

int main(){
	int trace;
	int numBridges;
	cin>>trace;
	cin>>numBridges;
	Bridge bridge[numBridges+1];
	//vector<string> LANSname[numBridges+1];
	map<string, LanSegment> lans;
	

	for(int i=1;i<numBridges+1;i++){
		int n;cin>>n;
		vector<pair<string,bool> > v;
		for(int j=0;j<n;j++){
			string name;cin>>name;
			//LANSname[i].push_back(name);
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
		bridge[i].PopulateBridge("B" + to_string(i),v);

	}

	/*for(int i=1;i<numBridges+1;i++){
		vector<pair<string,bool> > v;
		for(int j=0;j<LANSname[i].size();j++){
			v.push_back(make_pair(lans[LANSname[i][j]],true));
		}
		bridge[i].PopulateBridge("B" + to_string(i),v);
	}*/
	queue<Received> q;
	for(int i=1;i<numBridges+1;i++)
		bridge[i].SendConfigMessage(bridge,q,trace,lans);

	
	while(!q.empty()){
		Received temp = q.front();
		//temp.m.print();
		bridge[temp.id].ReceiveConfigMessage(temp.m,temp.time,temp.lan,bridge,q,trace,lans);
		q.pop();


	}
	//for(int i=1;i<numBridges+1;i++)
	
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


	/*map<string,string> hosts;
	for(int i=0;i<lans.size();i++){
		string lan;cin>>lan;
		int m;cin>>m;
		for(int j=0;j<m;j++){
			string h;cin>>h;
			hosts.insert(make_pair(h,lan));
		}
	}
	int T;cin>>T;
	for(int i=0;i<T;i++){
		string h1,h2;
		cin>>h1>>h2;
		for(int i=0;i<lans[hosts[h1]].bridges.size();i++){
			if(lans[hosts[h1]].bridges[i].second){
				int index = stoi(lans[hosts[h1]].bridges[i].first.substr(1));
				if(bridge[index].ForwardingTable.count(h1) == 0){
					bridge[index].ForwardingTable.insert(make_pair(h1,lans[hosts[h1]].name));
				}				 
				if(bridge[index].ForwardingTable.count(h2)>0){
					cout<<"Forward to" << bridge[index].ForwardingTable[h2]<<endl;
				}
				else{
					cout<<"Forward to all"<<endl;
				}
			}
		}
	}*/
	//cout<<lans["B"].bridges[1]<<endl;
	//cout<<LANSname[1][1]<<endl;
	//cout<<bridge[1].Lan_Segments[0].first.bridges[1]<<endl;
	
	return 0;
}