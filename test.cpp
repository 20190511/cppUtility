#include <iostream>
#include <vector>
#include <deque>
using namespace std;

pair<int,int> where;
deque<string> dq;

void bfs() {
	while(!dq.empty()) {
		string s = dq.front();
		dq.pop_front();	
		if (s.strstr(s.c_str(), "sha")) {
			dq.push_back(s + "Type : 1");
		}
	}
	
	return;
}
int main(void)
{
	printf("Hello World\n");
	dq.push_back(
	bfs();
	exit(0);
}
