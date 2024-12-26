#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<string>
#include<ctype.h>
#include<stack>
#include<map>
#include<algorithm>
#include <queue>
#include <cstdlib>
#include <cstdio>
#include <map>
#include <unordered_map>
using namespace std;

bool isEscapeChar(char c1, char c2) {
	if (c1 == '\\') {
		if (c2 == '(' || c2 == ')' || c2 == '[' || c2 == ']' || c2 == '*' || c2 == '|') {
			return true;
		}
	}
	return false;
}

char getEscapeChar(char c1, char c2) {
	return c2;
}

void prepareStr(string& s) {
	string temS;
	bool flag = false;
	stack<bool> st;
	for (int i = 0; i < s.size(); i++) {
		temS.push_back(s[i]);
		if (s[i] == '[') {
			temS.pop_back();
			temS.push_back('(');
			st.push(flag);
			flag = true;
		}
		if (s[i] == ']') {
			temS.pop_back();
			temS.push_back(')');
			flag = st.top();
			st.pop();
		}
		if (isalpha(s[i]) || isdigit(s[i]) || s[i] == ']' || s[i] == ')' || s[i] == '=' || s[i] == '>' || s[i] == '<' || s[i] == '!') {
			if (i < s.size() - 1) {
				if (isalpha(s[i + 1]) || isdigit(s[i + 1]) || s[i + 1] == '[' || s[i + 1] == '(' || s[i + 1] == '\\' || s[i + 1] == '=' || s[i + 1] == '>' || s[i + 1] == '<' || s[i + 1] == '!')
				{
					temS.push_back(flag ? '|' : '&');
				}
			}
		}//转义字符
		else if (s[i] == '\\') {
			i++;
			temS.push_back(s[i]);
			if (i < s.size() - 1 && s[i + 1] != '\\') {
				if (isalpha(s[i + 1]) || isdigit(s[i + 1]) || s[i + 1] == '[' || s[i + 1] == '(')
				{
					temS.push_back(flag ? '|' : '&');
				}
			}
			else if (i < s.size() - 1) {
				temS.push_back(flag ? '|' : '&');
			}
		}
	}

	//cout << temS << endl;
	//最后对减号处理
	s.clear();
	for (int i = 0; i < temS.size(); i++) {
		s.push_back(temS[i]);
		if (temS[i] == '-' && temS[i - 1] != '\\') {//需要将转义字符排除
			char startChar = temS[i - 1];
			char endChar = temS[i + 1];
			//cout << startChar << " " << endChar << endl;
			s.pop_back();
			s.pop_back();
			s.push_back('(');
			for (char i = startChar; i <= endChar; i++) {
				s.push_back(i);
				if (i != endChar)
					s.push_back('|');
			}
			s.push_back(')');
			i++;
		}
	}
}

int getNewNode() {
	static int n = 0;
	return n++;
}

class Edge {
public:
	int start;
	int end;
	char lable;
	Edge(int s, int e, char l) :start(s), end(e), lable(l) {}
};

class Graphy {
public:
	vector<Edge> edges;
	int start;
	int end;

	vector<int> getIn(int n) {
		vector<int> ret;
		for (auto it : edges) {
			if (it.end == n) {
				ret.push_back(it.lable);
			}
		}
		return ret;
	}

	vector<int> getOut(int n) {
		vector<int> ret;
		for (auto it : edges) {
			if (it.start == n) {
				ret.push_back(it.lable);
			}
		}
		return ret;
	}

	void addEdge(Edge edge) {
		edges.push_back(edge);
	}

	//改变状态 从s1出发经过c类型的路径能到达的节点
	vector<int> move(int s1, char c) {
		vector<int> ret;
		for (Edge edge : edges) {
			if (edge.start == s1 && edge.lable == c) {
				ret.push_back(edge.end);
			}
		}
		return ret;
	}
};

//连接两个图
void andGraphy(Graphy& g1, Graphy& g2) {
	//将g2和g1合并
	for (auto edge : g2.edges) {
		g1.edges.push_back(edge);
	}
	//添加新边 0 表示空边
	Edge newEdge(g1.end, g2.start, 0);
	g1.edges.push_back(newEdge);
	//更新g1其实和结束节点
	g1.end = g2.end;
}

//或两个图
void orGraphy(Graphy& g1, Graphy& g2) {
	//将g2和g1合并
	for (auto edge : g2.edges) {
		g1.edges.push_back(edge);
	}
	vector<int> in = g1.getIn(g1.end);
	vector<int> out = g1.getOut(g1.start);
	int flag = false;
	for (auto it : in) {
		if (it != 0) {
			flag = true;
		}
	}
	for (auto it : out) {
		if (it != 0) {
			flag = true;
		}
	}

	if ((!flag) && in.size() == out.size() && in.size() > 1) {
		//可快速处理
		g1.addEdge(Edge(g1.start, g2.start, 0));
		g1.addEdge(Edge(g2.end, g1.end, 0));
		//cout << "quick \n";
	}
	else {
		//不可快速处理
		int newStart = getNewNode();
		int newEnd = getNewNode();
		g1.addEdge(Edge(newStart, g1.start, 0));
		g1.addEdge(Edge(newStart, g2.start, 0));
		g1.addEdge(Edge(g1.end, newEnd, 0));
		g1.addEdge(Edge(g2.end, newEnd, 0));
		g1.start = newStart;
		g1.end = newEnd;
	}
}

void starGraphy(Graphy& g1) {
	int newStart = getNewNode();
	int newEnd = getNewNode();

	g1.addEdge(Edge(newStart, newEnd, 0));
	g1.addEdge(Edge(newStart, g1.start, 0));
	g1.addEdge(Edge(g1.end, g1.start, 0));
	g1.addEdge(Edge(g1.end, newEnd, 0));

	g1.start = newStart;
	g1.end = newEnd;
}

//仅把g2的边合并到g1
void combinGraphy_Simple(Graphy& g1, Graphy& g2) {
	//将g2和g1合并
	for (auto edge : g2.edges) {
		g1.edges.push_back(edge);
	}
}

//判断是否是运算符
bool isOpt(char a) {
	if (a == '&' || a == '|' || a == '*' || a == '(' || a == ')' || a == '[' || a == ']' || a == '-' || a == ' ') {
		return true;
	}
	return false;
}

Graphy charToGraphy(char c) {
	Graphy g;
	int start = getNewNode();
	int end = getNewNode();
	g.addEdge(Edge(start, end, c));
	g.start = start;
	g.end = end;
	return g;
}

bool cmpOpt(char op1, char op2) {
	static char table[] = { ' ',')','|','&','*','-','(' };
	int i = 0, j = 0;
	while (i < 5 && op1 != table[i]) i++;
	while (j < 5 && op2 != table[j]) j++;
	return i >= j;
}

void calculate(char op, stack<Graphy>& ope) {
	//空格符说明结束了
	if (op == ' ') return;

	if (op == '&') {
		if (ope.size() < 2) {
			printf("error &\n");
		}
		auto ope2 = ope.top();
		ope.pop();
		auto ope1 = ope.top();
		ope.pop();
		andGraphy(ope1, ope2);
		ope.push(ope1);
	}
	else if (op == '|') {
		if (ope.size() < 2) {
			printf("error |\n");
		}
		auto ope2 = ope.top();
		ope.pop();
		auto ope1 = ope.top();
		ope.pop();
		orGraphy(ope1, ope2);
		ope.push(ope1);
	}
	else if (op == '*') {
		if (ope.size() < 1) {
			printf("error *\n");
		}
		auto ope1 = ope.top();
		ope.pop();
		starGraphy(ope1);
		ope.push(ope1);
	}
}

Graphy reToNfa(string re) {
	stack<char> opt;//运算符栈
	stack<Graphy> ope;//图栈
	re.push_back(' ');
	for (int i = 0; i < re.size(); i++) {
		if (re[i] == '\\') {
			//处理转义字符
			i++;
			if (i >= re.size()) {
				i--;
			}
			Graphy newG = charToGraphy(re[i]);
			ope.push(newG);
		}
		else if (!isOpt(re[i])) {
			//不是运算符
			Graphy newG = charToGraphy(re[i]);
			ope.push(newG);
		}
		else {
			//是运算符
			if (re[i] == '(') {
				//如果是左括号就不比较直接进入
				opt.push(re[i]);
			}
			else {
				//如果是括号匹配
				if (re[i] == ')') {
					while (opt.size() != 0 && opt.top() != '(') {
						calculate(opt.top(), ope);
						opt.pop();
					}
					opt.pop();
				}
				else {
					//一直循环
					if (i == 54) {
						int a = 0;
					}
					while (opt.size() != 0 && opt.top() != '(' && opt.top() != '[' && opt.top() != '{' && cmpOpt(opt.top(), re[i])) {
						calculate(opt.top(), ope);
						opt.pop();
					}
					opt.push(re[i]);
				}
			}
		}

	}
	Graphy ret = ope.top();
	ope.pop();
	return ret;

}


int getNewNodeDfa() {
	static int num = 0;
	return num++;
}

//合并集合的方式合并两个容器
void connectVector(vector<int>& v1, vector<int>& v2) {
	int len = v2.size();
	for (int i = 0; i < len; i++) {
		int len2 = v1.size();
		bool flag = true;
		for (int j = 0; j < len2; j++) {
			if (v1[j] == v2[i]) {
				flag = false;
				break;
			}
		}
		if (flag) {
			v1.push_back(v2[i]);
		}
	}
}
//void connectVector(vector<int>& v1, vector<int>& v2) {
//	unordered_map<int, int> m;
//	for (auto it : v1) {
//		//if (m.find(it) == m.end()) continue;
//		m.insert({ it, 1 });
//	}
//	for (auto it : v2) {
//		auto flag = m.find(it);
//		if (flag == m.end()) {
//			v1.push_back(it);
//		}
//	}
//}

//将v2 合并到 v1中 去重
void combineVectorUniq(vector<int>& v1, vector<int>& v2) {
	for (int i = 0; i < v2.size(); i++) {
		bool flag = true;
		for (int j = 0; j < v1.size(); j++) {
			if (v1[j] == v2[i]) {
				flag = false;
				break;
			}
		}
		if (!flag) continue;
		v1.push_back(v2[i]);
	}
}
//void combineVectorUniq(vector<int>& v1, vector<int>& v2) {
//	unordered_map<int, int> m;
//	for (auto it : v1) {
//		//if (m.find(it) == m.end()) continue;
//		m.insert({ it, 1 });
//	}
//	for (auto it : v2) {
//		auto flag = m.find(it);
//		if (flag == m.end()) {
//			v1.push_back(it);
//		}
//	}
//}


int findVectorInTable(vector<vector<int>>& table, vector<int>& v) {
	sort(v.begin(), v.end());
	int pos = -1;
	for (auto& temV : table) {
		pos++;
		sort(temV.begin(), temV.end());
		if (temV.size() != v.size()) {
			continue;
		}
		bool flag = true;
		for (int i = 0; i < temV.size(); i++) {
			if (temV[i] != v[i]) {
				flag = false;
				break;
			}
		}
		if (!flag) continue;

		return pos;
	}
	return -1;
}

vector<char> symbols = {
	   'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
	   'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	   'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
	   'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	   '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	   '(', ')', '{', '}', '[', ']', '+', '-', '*', '/', '=','!',':',';',
	   '<','>','?'
};

//求空边闭包
void emptyClosures(Graphy& g, vector<int>& v) {
	//求空边闭包
	bool flag = true;
	vector<int> empty;
	int preLen = v.size();
	while (flag) {
		flag = false;
		for (int k = 0; k < v.size(); k++) {
			empty = g.move(v[k], 0);
			combineVectorUniq(v, empty);
		}
		if (preLen != v.size()) {
			flag = true;
			preLen = v.size();
		}
	}
}


//记录nfa中每一个状态对应dfa状态的集合
vector<vector<int>> table1;

Graphy nfaToDfa(Graphy nfa) {
	Graphy dfa;

	//先求nfa 起始节点空边闭包
	int node = getNewNodeDfa();
	vector<int> set1 = nfa.move(nfa.start, 0);
	set1.push_back(nfa.start);
	emptyClosures(nfa, set1);

	table1.push_back(set1);

	sort(set1.begin(), set1.end());
	/*for (auto it : set1) {
		cout << it << " ";
	}*/
	//cout << endl;
	int start = nfa.start;
	int end = nfa.end;

	queue <int> q;
	q.push(node);

	while (q.size()) {
		int tem = q.front();
		q.pop();
		vector<int> t = table1[tem];
		for (int i = 0; i < symbols.size(); i++) {
			vector<int> next;
			for (int j = 0; j < t.size(); j++) {
				vector<int> next1 = nfa.move(t[j], symbols[i]);
				combineVectorUniq(next, next1);
			}
			emptyClosures(nfa, next);
			int newVec = 0;
			if ((newVec = findVectorInTable(table1, next)) == -1) {//检查是否是新状态
				//新状态 添加进table1
				table1.push_back(next);
				int newNode = getNewNodeDfa();
				dfa.addEdge(Edge(tem, newNode, symbols[i]));
				q.push(newNode);
			}
			else if (next.size() != 0) {
				dfa.addEdge(Edge(tem, newVec, symbols[i]));
			}
		}
	}
	return dfa;
}





int getIndexInSymble(char c) {
	for (int i = 0; i < symbols.size(); i++) {
		if (symbols[i] == c) return i;
	}
	return -1;
}


bool wordPriCmp(string& s1, string& s2) {
	vector<string> table = { "res", "id","num","sigle","dword" };
	int pr1 = 0;
	int pr2 = 0;
	for (pr1 = 0; pr1 < table.size(); pr1++) {
		if (table[pr1] == s1) {
			break;
		}
	}
	for (pr2 = 0; pr2 < table.size(); pr2++) {
		if (table[pr2] == s2) {
			break;
		}
	}
	return pr1 <= pr2;
}
//最小化dfa
Graphy simplifyDfa(Graphy& dfa, vector<string>& endStatuTable, vector<vector<int>>& trans) {
	// 先划分成终态和非终态
	vector<int>* finalState = new vector<int>;
	vector<int>* unFinalState = new vector<int>;

	map<int, vector<int>*> t1; //每个元素当前在哪个集合
	vector<vector<int>*> setV;//统计遍历过的集合，主要用于释放内存
	setV.push_back(finalState);
	setV.push_back(unFinalState);

	//人为添加一种状态转换，避免不用意义的终态被合并成一个
	//每个终态都有自己的意义，有的代表是保留字，有的代表是数字等等
	// 经过实验在合并过程中，会发生不同意义的终态被合并成一个（保留字会和id合并到一个<= 和 <等也会被合并）
	//为不同意义的终态创建新节点，用属性为-1的边连接（-1的原因是符号表中没有ascii码的字符）
	//通过这种方法，有 -1 一定能将不同属性的终态划分到不同集合中
	map<string, int> esIndex;//每个终态都有自己的意义，不同意义对应的新节点
	for (auto s : endStatuTable) {
		if (esIndex.find(s) == esIndex.end()) {
			esIndex[s] = getNewNodeDfa();
		}
	}

	int endStatuNum = 0;
	for (int i = 0; i < table1.size(); i++) {
		if (endStatuTable[i] != "none") {
			//终态
			finalState->push_back(i);
			t1[i] = finalState;
			endStatuNum++;
			trans[i].push_back(esIndex[endStatuTable[i]]);
		}
		else {
			//非终态
			unFinalState->push_back(i);
			t1[i] = unFinalState;
			trans[i].push_back(-1);
		}
	}

	queue<vector<int>*> q;

	q.push(unFinalState);
	q.push(finalState);

	symbols.push_back(-1);
	bool flag = true;
	while (flag) {//不断用符号表划分，直到没有新的集合出现
		flag = false;
		//遍历符号表
		for (int s = 0; s < symbols.size(); s++) {
			int len = q.size();
			for (int i = 0; i < len; i++) {
				// 会更据当前集合元素通过某种符号转换后到达哪个集合进行划分
				// m的前一个vector<int>* 代表这状态转换到达哪个集合
				// m的后一个vector<int>* 表示当前集合用s划分后，到达这集合的元素有哪些
				map<vector<int>*, vector<int>*> m;
				vector<int>* tem = q.front();
				q.pop();
				for (auto elem : *tem) {
					int next = trans[elem][s];
					vector<int>* nextV = t1[next];
					if (m.find(nextV) == m.end()) {
						m[nextV] = new vector<int>;
						//防止内存泄漏，统一释放
						setV.push_back(m[nextV]);
					}
					m[nextV]->push_back(elem);
					//更新当前元素对应的集合
					t1[elem] = m[nextV];
				}
				if (m.size() != 1) {
					//更新了
					flag = true;
				}
				for (auto elem : m) {
					//将新划分出的集合加入到队列中
					q.push(elem.second);
				}
			}

		}
	}
	symbols.pop_back();

	map<int, int> m;
	map<int, int> m2;
	vector<string> newEndStatuTable(q.size(), "none");
	int newNodeIndex = 1;
	//计算每个状态用哪个状态替换，如果没有就是不替换（用自己替换自己）
	while (q.size()) {
		auto tem = q.front();
		q.pop();
		int val = (*tem)[0];
		if (val != 0)
			m2[val] = newNodeIndex++;
		else {
			m2[val] = 0;//初态必须要单独写出来
		}

		//newEndStatuTable.push_back(endStatuTable[val]);
		newEndStatuTable[m2[val]] = endStatuTable[val];
		for (auto elem : *tem) {
			//cout << elem << " ";
			m[elem] = val;
		}
		//cout << endl << endl;
	}
	m[-1] = -1;
	for (int i = 0; i < trans.size(); i++) {
		if (m.find(i) == m.end()) continue;
		if (m[i] == i) {
			for (int j = 0; j < trans[i].size() - 1; j++) {
				if (trans[i][j] == -1) {
					int a = 0;
				}
				trans[i][j] = m[trans[i][j]];
			}
		}
		else {
			for (int j = 0; j < trans[i].size() - 1; j++) {
				trans[i][j] = -1;
			}
		}
	}



	Graphy simG;
	for (int i = 0; i < trans.size(); i++) {
		if (m2.find(i) == m2.end()) {
			continue;
		}

		for (int j = 0; j < trans[i].size() - 1; j++) {//-1 是因为之前人为多加了一项
			if (trans[i][j] != -1) {
				if (m2.find(trans[i][j]) == m2.end()) {
					continue;
				}
				simG.addEdge(Edge(m2[i], m2[trans[i][j]], symbols[j]));
			}
			else {
				int a = 0;
			}
		}
	}
	vector<vector<int>> newTrans(newNodeIndex, vector<int>(symbols.size(), -1));
	int a = 0;
	for (auto edge : simG.edges) {
		newTrans[edge.start][getIndexInSymble(edge.lable)] = edge.end;
	}

	trans = newTrans;
	endStatuTable = newEndStatuTable;
	return simG;
}

const char* prepareBufFunctionSource =
"int prepareBuf(char* buf) {\n"
"    //处理掉注释和无效字符\n"
"    char temBuf[4096];\n"
"    //两个索引\n"
"    int runPos = 0;//用于访问buf\n"
"    int runTem = 0;//用于填充temBuf\n"
"    int curLine = 1;//记录当前字符所在行，遇到换行符会更新\n"
"    while (runPos < pos) {\n"
"        //处理注释\n"
"        if (buf[runPos] == '/' && buf[runPos+1] == '*') {\n"
"            int lineExegesis = curLine;\n"
"            while (runPos < pos - 1 && (buf[runPos] != '*' || buf[runPos + 1] != '/'))\n"
"            {\n"
"                if (buf[runPos] == '\\n') curLine++;\n"
"                runPos++;\n"
"            }\n"
"            if (buf[runPos] != '*' || buf[runPos + 1] != '/') {\n"
"                printf(\"第%%d行 注释错了 没有找到结束符号(*/) \\n\",lineExegesis);\n"
"                //添加结束符号\n"
"                temBuf[runTem] = 0;\n"
"                //将结果复制给全局的输入缓冲区\n"
"                strcpy(buf, temBuf);\n"
"                //更新结束索引\n"
"                pos = runTem;\n"
"                return -1;\n"
"            }\n"
"            runPos += 2;\n"
"        }\n"
"        else if (buf[runPos] != '\\n' && buf[runPos] != '\\t' && buf[runPos] != '\\v' && buf[runPos] != '\\r') {\t\n"
"            //普通文本 添加进临时缓冲区\n"
"            lineTable[runTem] = curLine;\n"
"            temBuf[runTem++] = buf[runPos++];\n"
"        }\n"
"        else\n"
"        {\n"
"            //去除无用字符\n"
"            if (buf[runPos] == '\\n') curLine++;\n"
"            runPos++;\n"
"        }\n"
"    }\n"
"    //添加结束符号\n"
"    temBuf[runTem] = 0;\n"
"    //将结果复制给全局的输入缓冲区\n"
"    strcpy(buf, temBuf);\n"
"    //更新结束索引\n"
"    pos = runTem ;\n"
"    return 1;\n"
"}\n";

const char* isEndStatuSrc = "int isEndStatu(int statu) {\n"
"    for (int i = 0; i < endStatuTableSize; i++) {\n"
"        if (endStatuTable[i] == statu) {\n"
"            return 1;\n"
"        }\n"
"    }\n"
"    return 0;\n"
"}\n";

const char* skipSpace = "void skipSpace() {\n"
"    while (scanPos < pos && buf[scanPos] == ' ') {\n"
"        scanPos++;\n"
"    }\n"
"}\n";

const char* isBorden = "int isBorden(char c) {\n"
"    return !isdigit(c) && !isalpha(c);\n"
"}\n";



const char* scanSrc =
"int scan(int* endStatu, char word[]) {\n"
"    int curStatu = 0;\n"
"    int preStatu = 0;\n"
"    int tokenPos = 0;\n"
"    char token[4096];\n"
"\n"
"    // 去除空格等字符\n"
"    while (scanPos < pos && getIndexChar(buf[scanPos]) == -1) {\n"
"        if (buf[scanPos] != ' ' && buf[scanPos] != '#')\n"
"            printf(\"in %%d line invalid char %%c\\n\", lineTable[scanPos], buf[scanPos]);\n"
"        scanPos++;\n"
"    }\n"
"\n"
"    // 使用状态机进行分析\n"
"    while (getIndexChar(buf[scanPos]) != -1 && curStatu != -1 && scanPos < pos) {\n"
"        preStatu = curStatu;\n"
"        curStatu = trans[curStatu][getIndexChar(buf[scanPos])];\n"
"        token[tokenPos++] = buf[scanPos++];\n"
"    }\n"
"\n"
"    // 对结果进行判断\n"
"    if (curStatu == -1) {\n"
"        // 首先判断前一个状态是否是终态，在本状态机中除了0以外都是终态\n"
"        if (!isEndStatu(preStatu)) { // 是否是终态的判断\n"
"            token[tokenPos] = 0;\n"
"            printf(\"error word %%s\\n\", token);\n"
"            strcpy(word, token);\n"
"            *endStatu = -1;\n"
"        }\n"
"        else {\n"
"if (!isBorden(token[tokenPos - 1]) && !isBorden(token[tokenPos - 2])) {\n"
"	//还是非法连接\n"
"	token[tokenPos] = 0;\n"
"	printf(\"error word %%s\\n\", token);\n"
"   strcpy(word, token);\n"
"   *endStatu = -1;\n"
"	return -1;\n"
"}\n"
"else {\n"
"	//合法的连接\n"
"	token[tokenPos - 1] = 0;\n"
"	scanPos--;\n"
"	*endStatu = preStatu;\n"
"	strcpy(word, token);\n"
"	return 1;\n"
"}\n"
"}\n"
"        *endStatu = preStatu;\n"
"        strcpy(word, token);\n"
"        return 1;\n"
"    }\n"
"\n"
"    token[tokenPos] = 0;\n"
"    *endStatu = curStatu;\n"
"    strcpy(word, token);\n"
"\n"
"    return 1;\n"
"}\n";


const char* mainSrc = "int main()\n"
"{\n"
"    FILE* file = fopen(\"temp2.txt\", \"r\");\n"
"    char temChar;\n"
"\n"
"    while ((temChar = fgetc(file)) != EOF) {\n"
"        buf[pos++] = temChar;\n"
"    }\n"
"    buf[pos] = '\\0';\n"
"\n"
"    fclose(file);\n"
"    printf(\"read file: \\n%%s\\n\", buf);\n"  // 注意这里的 %% 用于表示字符串中的 %\n"
"    prepareBuf(buf);\n"
"    pos = strlen(buf);\n"
"    //添加结束符\n"
"    strcat(buf, \"#\");\n"
"    printf(\"after prepare:\\n%%s\", buf);\n"  // 注意这里的 %% 用于表示字符串中的 %\n"
"    printf(\"\\n\\nstart scan\\n\\n\");\n"
"    while (buf[scanPos] != '#') {\n"
"        char word[4096];\n"
"        int statu = 0;\n"
"        scan(&statu, word);\n"
"        if(statu==-1) continue;\n"
"        output(statu, word);\n"
"    }\n"
"    return 0;\n"
"}\n";


//头文件最长长度 包含#include
#define MAX_HEAD_LEN 30
//配置文件读取缓冲区
char bufConf[1024];
int posConf = 0;


int main()
{
	FILE* file = fopen("test.txt", "r");
	char buf[1024];
	vector<vector<string>> re;
	//获取正则表达式
	while ((fgets(buf, 1024, file) != NULL)) {
		if (strlen(buf) == 0) continue;
		if (strstr(buf, "=") == NULL) continue;
		int begin = 0;
		while (buf[begin] == ' ') begin++;
		int end = begin;
		while (buf[end] != '=' && buf[end] != ' ') {
			end++;
		}
		string name = "";
		for (int i = begin; i < end; i++) {
			name.push_back(buf[i]);
		}
		if (buf[end] != '=') {
			while (end < strlen(buf) && buf[end] != '=') end++;
		}

		end++;
		while (end < strlen(buf) && buf[end] == ' ') end++;
		begin = end;
		while (end < strlen(buf) && buf[end] != ' ') end++;
		string val = "";
		for (int i = begin; i < end; i++) {
			if (buf[i] == '\n' && i == end - 1) continue;
			val.push_back(buf[i]);
		}
		re.push_back({ name,val });
	}

	cout << "读取正则表达式如下:\n";
	for (auto it : re) {
		cout << it[0] << "\t\t" << it[1] << endl;
	}
	fclose(file);

	vector<Graphy> nfa(re.size());
	cout << "预处理后的正则表达式\n";
	for (int i = 0; i < re.size(); i++) {
		prepareStr(re[i][1]);
		cout << re[i][1] << endl;
	}
	cout << "开始将正则表达式翻译成nfa\n";
	for (int i = 0; i < re.size(); i++) {
		nfa[i] = reToNfa(re[i][1]);
	}
	cout << "翻译结束\n";

	//合并得到的nfa
	for (int i = 1; i < nfa.size(); i++) {
		combinGraphy_Simple(nfa[0], nfa[i]);
	}
	int newNode = getNewNode();
	for (int i = 0; i < nfa.size(); i++) {
		nfa[0].addEdge(Edge(newNode, nfa[i].start, 0));
	}
	nfa[0].start = newNode;

	cout << "开始将nfa转化成dfa\n";
	Graphy dfa = nfaToDfa(nfa[0]);
	//得到转换图
	vector<vector<int>> trans(table1.size(), vector<int>(symbols.size(), -1));
	for (auto edge : dfa.edges) {
		trans[edge.start][getIndexInSymble(edge.lable)] = edge.end;
	}
	cout << "nfa转化结束\n";

	////合成终态
	vector<int> nfaEndStatu(nfa.size());
	for (int i = 0; i < nfa.size(); i++) {
		nfaEndStatu[i] = nfa[i].end;
	}

	vector<string> endStatuTable(table1.size(), "none");
	for (int i = 0; i < table1.size(); i++) {
		int set_statu = -1;
		endStatuTable[i] = "none";
		for (int j = 0; j < table1[i].size(); j++) {
			bool flag = false;
			//查看当前状态是否是终态
			int k = 0;
			for (k = 0; k < nfaEndStatu.size(); k++) {
				if (nfaEndStatu[k] == table1[i][j]) {
					flag = true;
					break;
				}
			}
			if (flag) {
				//是终态
				endStatuTable[i] = re[k][0];
			}
		}
	}
	cout << "开始化简dfa\n";
	simplifyDfa(dfa, endStatuTable, trans);
	cout << "化简结束\n";

	//开始合成词法分析器程序
	FILE* tarFile = fopen("lex.c", "w");

	cout << "写入头文件\n";
	//先写入头文件
	fprintf(tarFile, "#include<stdio.h>\n");
	fprintf(tarFile, "#include<stdlib.h>\n");
	fprintf(tarFile, "#include<string.h>\n");
	fprintf(tarFile, "#include<ctype.h>\n");

	cout << "写入符号表\n";
	//添加符号表
	fprintf(tarFile, "char symble[]={\n");
	for (int i = 0; i < symbols.size(); i++) {
		fprintf(tarFile, i == symbols.size() - 1 ? "\'%c\'" : "\'%c\',", symbols[i]);
	}
	fprintf(tarFile, "};\n");

	//添加符号表的信息
	fprintf(tarFile, "int lenSym = %d;\n", symbols.size());

	//添加转换表
	fprintf(tarFile, "int trans[][%d]={\n", trans[0].size());
	for (int j = 0; j < trans.size(); j++) {
		fprintf(tarFile, "{ ");
		for (int i = 0; i < trans[j].size(); i++) {
			fprintf(tarFile, i == trans[j].size() - 1 ? "%d" : "%d,", trans[j][i]);
		}
		fprintf(tarFile, j == trans.size() - 1 ? " }\n" : " },\n");
	}
	fprintf(tarFile, "};");

	cout << "添加终态表\n";
	//添加终态表
	fprintf(tarFile, "int endStatuTable[%d]={\n", endStatuTable.size());
	for (int i = 0; i < endStatuTable.size(); i++) {
		if (endStatuTable[i] == "none") continue;
		fprintf(tarFile, i == endStatuTable.size() ? "%d" : "%d,", i);
	}
	fprintf(tarFile, "};\n");
	fprintf(tarFile, "int endStatuTableSize=%d;\n", endStatuTable.size());


	cout << "添加缓冲区\n";
	//添加全局变量
	//1. 输入缓冲区
	fprintf(tarFile, "char buf[4096];\n");
	//2. 行缓冲区
	fprintf(tarFile, "int lineTable[4096];\n");
	//3. 缓冲区索引
	fprintf(tarFile, "int pos = 0;\n");

	cout << "添加必要功能函数\n";
	cout << "getIndexChar\n" << "prepareBuf\n" << "skipSpace\n" << "isEndStatu\n" << "isBorden\n" << "scan\n" << "output\n";
	cout << "main\n";
	//添加必要的功能函数
	//1. 添加函数 int getIndexChar（char）；
	fprintf(tarFile, "int getIndexChar(char c){\n");
	fprintf(tarFile, "int i = 0;\n for(i=0;i<lenSym;i++){\nif(symble[i]==c){\n return i;\n}\n}\nreturn -1;");
	fprintf(tarFile, "}\n");

	//2. 添加函数 int prepareBuf(char* buf)
	fprintf(tarFile, prepareBufFunctionSource);

	//添加变量 int scanPos = 0;
	fprintf(tarFile, "int scanPos = 0;\n");
	//3. 添加函数 void skipSpace() 跳过空格
	fprintf(tarFile, skipSpace);
	//4. 添加函数 判断当前状态是否是终态 int isEndStatu(int statu);
	fprintf(tarFile, isEndStatuSrc);
	//添加isBorden
	fprintf(tarFile, isBorden);
	//5. 将扫描函数加入
	fprintf(tarFile, scanSrc);
	//6. 添加函数output
	fprintf(tarFile, "void output(int statu,char word[]){\n");
	fprintf(tarFile, "switch(statu){\n");
	for (int i = 0; i < endStatuTable.size(); i++) {
		if (endStatuTable[i] == "none") continue;
		fprintf(tarFile, "case %d:\n", i);
		if (endStatuTable[i] != string("NUM") && endStatuTable[i] != string("ID")) {
			fprintf(tarFile, "printf(\"(%%s,%%s)\\n\",word,word);\n");
		}
		else
			fprintf(tarFile, "printf(\"(%s,%%s)\\n\",word);\n", endStatuTable[i].c_str());
		fprintf(tarFile, "break;\n");
	}
	fprintf(tarFile, "};\n");
	fprintf(tarFile, "}\n");

	//添加主函数
	fprintf(tarFile, mainSrc);
	fclose(tarFile);
	cout << "词法分析器已创建成功 文件名为lex.c\n";


	return 0;
}


/*

sigle = +|\-|\*|/|=|\(|\)|{|}|:|,|;|<|>|!
dwrod = >=|<=|==|<>
ID = [a-zA-Z][a-zA-Z0-9]*
NUM = num[0-9]*；
RES = import|package|main|show|char|for
*/

/*
sigle = +|\-|\*|/|=|\(|\)|{|}|:|,|;|<|>|!
dwrod = >=|<=|==|!=
ID = [a-zA-Z][a-zA-Z0-9]*
NUM =  [0-9]*
RES = if|else|for|while|do|int|write|read

*/