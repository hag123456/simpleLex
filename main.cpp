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
		}//ת���ַ�
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
	//���Լ��Ŵ���
	s.clear();
	for (int i = 0; i < temS.size(); i++) {
		s.push_back(temS[i]);
		if (temS[i] == '-' && temS[i - 1] != '\\') {//��Ҫ��ת���ַ��ų�
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

	//�ı�״̬ ��s1��������c���͵�·���ܵ���Ľڵ�
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

//��������ͼ
void andGraphy(Graphy& g1, Graphy& g2) {
	//��g2��g1�ϲ�
	for (auto edge : g2.edges) {
		g1.edges.push_back(edge);
	}
	//����±� 0 ��ʾ�ձ�
	Edge newEdge(g1.end, g2.start, 0);
	g1.edges.push_back(newEdge);
	//����g1��ʵ�ͽ����ڵ�
	g1.end = g2.end;
}

//������ͼ
void orGraphy(Graphy& g1, Graphy& g2) {
	//��g2��g1�ϲ�
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
		//�ɿ��ٴ���
		g1.addEdge(Edge(g1.start, g2.start, 0));
		g1.addEdge(Edge(g2.end, g1.end, 0));
		//cout << "quick \n";
	}
	else {
		//���ɿ��ٴ���
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

//����g2�ıߺϲ���g1
void combinGraphy_Simple(Graphy& g1, Graphy& g2) {
	//��g2��g1�ϲ�
	for (auto edge : g2.edges) {
		g1.edges.push_back(edge);
	}
}

//�ж��Ƿ��������
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
	//�ո��˵��������
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
	stack<char> opt;//�����ջ
	stack<Graphy> ope;//ͼջ
	re.push_back(' ');
	for (int i = 0; i < re.size(); i++) {
		if (re[i] == '\\') {
			//����ת���ַ�
			i++;
			if (i >= re.size()) {
				i--;
			}
			Graphy newG = charToGraphy(re[i]);
			ope.push(newG);
		}
		else if (!isOpt(re[i])) {
			//���������
			Graphy newG = charToGraphy(re[i]);
			ope.push(newG);
		}
		else {
			//�������
			if (re[i] == '(') {
				//����������žͲ��Ƚ�ֱ�ӽ���
				opt.push(re[i]);
			}
			else {
				//���������ƥ��
				if (re[i] == ')') {
					while (opt.size() != 0 && opt.top() != '(') {
						calculate(opt.top(), ope);
						opt.pop();
					}
					opt.pop();
				}
				else {
					//һֱѭ��
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

//�ϲ����ϵķ�ʽ�ϲ���������
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

//��v2 �ϲ��� v1�� ȥ��
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

//��ձ߱հ�
void emptyClosures(Graphy& g, vector<int>& v) {
	//��ձ߱հ�
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


//��¼nfa��ÿһ��״̬��Ӧdfa״̬�ļ���
vector<vector<int>> table1;

Graphy nfaToDfa(Graphy nfa) {
	Graphy dfa;

	//����nfa ��ʼ�ڵ�ձ߱հ�
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
			if ((newVec = findVectorInTable(table1, next)) == -1) {//����Ƿ�����״̬
				//��״̬ ��ӽ�table1
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
//��С��dfa
Graphy simplifyDfa(Graphy& dfa, vector<string>& endStatuTable, vector<vector<int>>& trans) {
	// �Ȼ��ֳ���̬�ͷ���̬
	vector<int>* finalState = new vector<int>;
	vector<int>* unFinalState = new vector<int>;

	map<int, vector<int>*> t1; //ÿ��Ԫ�ص�ǰ���ĸ�����
	vector<vector<int>*> setV;//ͳ�Ʊ������ļ��ϣ���Ҫ�����ͷ��ڴ�
	setV.push_back(finalState);
	setV.push_back(unFinalState);

	//��Ϊ���һ��״̬ת�������ⲻ���������̬���ϲ���һ��
	//ÿ����̬�����Լ������壬�еĴ����Ǳ����֣��еĴ��������ֵȵ�
	// ����ʵ���ںϲ������У��ᷢ����ͬ�������̬���ϲ���һ���������ֻ��id�ϲ���һ��<= �� <��Ҳ�ᱻ�ϲ���
	//Ϊ��ͬ�������̬�����½ڵ㣬������Ϊ-1�ı����ӣ�-1��ԭ���Ƿ��ű���û��ascii����ַ���
	//ͨ�����ַ������� -1 һ���ܽ���ͬ���Ե���̬���ֵ���ͬ������
	map<string, int> esIndex;//ÿ����̬�����Լ������壬��ͬ�����Ӧ���½ڵ�
	for (auto s : endStatuTable) {
		if (esIndex.find(s) == esIndex.end()) {
			esIndex[s] = getNewNodeDfa();
		}
	}

	int endStatuNum = 0;
	for (int i = 0; i < table1.size(); i++) {
		if (endStatuTable[i] != "none") {
			//��̬
			finalState->push_back(i);
			t1[i] = finalState;
			endStatuNum++;
			trans[i].push_back(esIndex[endStatuTable[i]]);
		}
		else {
			//����̬
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
	while (flag) {//�����÷��ű��֣�ֱ��û���µļ��ϳ���
		flag = false;
		//�������ű�
		for (int s = 0; s < symbols.size(); s++) {
			int len = q.size();
			for (int i = 0; i < len; i++) {
				// ����ݵ�ǰ����Ԫ��ͨ��ĳ�ַ���ת���󵽴��ĸ����Ͻ��л���
				// m��ǰһ��vector<int>* ������״̬ת�������ĸ�����
				// m�ĺ�һ��vector<int>* ��ʾ��ǰ������s���ֺ󣬵����⼯�ϵ�Ԫ������Щ
				map<vector<int>*, vector<int>*> m;
				vector<int>* tem = q.front();
				q.pop();
				for (auto elem : *tem) {
					int next = trans[elem][s];
					vector<int>* nextV = t1[next];
					if (m.find(nextV) == m.end()) {
						m[nextV] = new vector<int>;
						//��ֹ�ڴ�й©��ͳһ�ͷ�
						setV.push_back(m[nextV]);
					}
					m[nextV]->push_back(elem);
					//���µ�ǰԪ�ض�Ӧ�ļ���
					t1[elem] = m[nextV];
				}
				if (m.size() != 1) {
					//������
					flag = true;
				}
				for (auto elem : m) {
					//���»��ֳ��ļ��ϼ��뵽������
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
	//����ÿ��״̬���ĸ�״̬�滻�����û�о��ǲ��滻�����Լ��滻�Լ���
	while (q.size()) {
		auto tem = q.front();
		q.pop();
		int val = (*tem)[0];
		if (val != 0)
			m2[val] = newNodeIndex++;
		else {
			m2[val] = 0;//��̬����Ҫ����д����
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

		for (int j = 0; j < trans[i].size() - 1; j++) {//-1 ����Ϊ֮ǰ��Ϊ�����һ��
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
"    //�����ע�ͺ���Ч�ַ�\n"
"    char temBuf[4096];\n"
"    //��������\n"
"    int runPos = 0;//���ڷ���buf\n"
"    int runTem = 0;//�������temBuf\n"
"    int curLine = 1;//��¼��ǰ�ַ������У��������з������\n"
"    while (runPos < pos) {\n"
"        //����ע��\n"
"        if (buf[runPos] == '/' && buf[runPos+1] == '*') {\n"
"            int lineExegesis = curLine;\n"
"            while (runPos < pos - 1 && (buf[runPos] != '*' || buf[runPos + 1] != '/'))\n"
"            {\n"
"                if (buf[runPos] == '\\n') curLine++;\n"
"                runPos++;\n"
"            }\n"
"            if (buf[runPos] != '*' || buf[runPos + 1] != '/') {\n"
"                printf(\"��%%d�� ע�ʹ��� û���ҵ���������(*/) \\n\",lineExegesis);\n"
"                //��ӽ�������\n"
"                temBuf[runTem] = 0;\n"
"                //��������Ƹ�ȫ�ֵ����뻺����\n"
"                strcpy(buf, temBuf);\n"
"                //���½�������\n"
"                pos = runTem;\n"
"                return -1;\n"
"            }\n"
"            runPos += 2;\n"
"        }\n"
"        else if (buf[runPos] != '\\n' && buf[runPos] != '\\t' && buf[runPos] != '\\v' && buf[runPos] != '\\r') {\t\n"
"            //��ͨ�ı� ��ӽ���ʱ������\n"
"            lineTable[runTem] = curLine;\n"
"            temBuf[runTem++] = buf[runPos++];\n"
"        }\n"
"        else\n"
"        {\n"
"            //ȥ�������ַ�\n"
"            if (buf[runPos] == '\\n') curLine++;\n"
"            runPos++;\n"
"        }\n"
"    }\n"
"    //��ӽ�������\n"
"    temBuf[runTem] = 0;\n"
"    //��������Ƹ�ȫ�ֵ����뻺����\n"
"    strcpy(buf, temBuf);\n"
"    //���½�������\n"
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
"    // ȥ���ո���ַ�\n"
"    while (scanPos < pos && getIndexChar(buf[scanPos]) == -1) {\n"
"        if (buf[scanPos] != ' ' && buf[scanPos] != '#')\n"
"            printf(\"in %%d line invalid char %%c\\n\", lineTable[scanPos], buf[scanPos]);\n"
"        scanPos++;\n"
"    }\n"
"\n"
"    // ʹ��״̬�����з���\n"
"    while (getIndexChar(buf[scanPos]) != -1 && curStatu != -1 && scanPos < pos) {\n"
"        preStatu = curStatu;\n"
"        curStatu = trans[curStatu][getIndexChar(buf[scanPos])];\n"
"        token[tokenPos++] = buf[scanPos++];\n"
"    }\n"
"\n"
"    // �Խ�������ж�\n"
"    if (curStatu == -1) {\n"
"        // �����ж�ǰһ��״̬�Ƿ�����̬���ڱ�״̬���г���0���ⶼ����̬\n"
"        if (!isEndStatu(preStatu)) { // �Ƿ�����̬���ж�\n"
"            token[tokenPos] = 0;\n"
"            printf(\"error word %%s\\n\", token);\n"
"            strcpy(word, token);\n"
"            *endStatu = -1;\n"
"        }\n"
"        else {\n"
"if (!isBorden(token[tokenPos - 1]) && !isBorden(token[tokenPos - 2])) {\n"
"	//���ǷǷ�����\n"
"	token[tokenPos] = 0;\n"
"	printf(\"error word %%s\\n\", token);\n"
"   strcpy(word, token);\n"
"   *endStatu = -1;\n"
"	return -1;\n"
"}\n"
"else {\n"
"	//�Ϸ�������\n"
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
"    printf(\"read file: \\n%%s\\n\", buf);\n"  // ע������� %% ���ڱ�ʾ�ַ����е� %\n"
"    prepareBuf(buf);\n"
"    pos = strlen(buf);\n"
"    //��ӽ�����\n"
"    strcat(buf, \"#\");\n"
"    printf(\"after prepare:\\n%%s\", buf);\n"  // ע������� %% ���ڱ�ʾ�ַ����е� %\n"
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


//ͷ�ļ������ ����#include
#define MAX_HEAD_LEN 30
//�����ļ���ȡ������
char bufConf[1024];
int posConf = 0;


int main()
{
	FILE* file = fopen("test.txt", "r");
	char buf[1024];
	vector<vector<string>> re;
	//��ȡ������ʽ
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

	cout << "��ȡ������ʽ����:\n";
	for (auto it : re) {
		cout << it[0] << "\t\t" << it[1] << endl;
	}
	fclose(file);

	vector<Graphy> nfa(re.size());
	cout << "Ԥ������������ʽ\n";
	for (int i = 0; i < re.size(); i++) {
		prepareStr(re[i][1]);
		cout << re[i][1] << endl;
	}
	cout << "��ʼ��������ʽ�����nfa\n";
	for (int i = 0; i < re.size(); i++) {
		nfa[i] = reToNfa(re[i][1]);
	}
	cout << "�������\n";

	//�ϲ��õ���nfa
	for (int i = 1; i < nfa.size(); i++) {
		combinGraphy_Simple(nfa[0], nfa[i]);
	}
	int newNode = getNewNode();
	for (int i = 0; i < nfa.size(); i++) {
		nfa[0].addEdge(Edge(newNode, nfa[i].start, 0));
	}
	nfa[0].start = newNode;

	cout << "��ʼ��nfaת����dfa\n";
	Graphy dfa = nfaToDfa(nfa[0]);
	//�õ�ת��ͼ
	vector<vector<int>> trans(table1.size(), vector<int>(symbols.size(), -1));
	for (auto edge : dfa.edges) {
		trans[edge.start][getIndexInSymble(edge.lable)] = edge.end;
	}
	cout << "nfaת������\n";

	////�ϳ���̬
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
			//�鿴��ǰ״̬�Ƿ�����̬
			int k = 0;
			for (k = 0; k < nfaEndStatu.size(); k++) {
				if (nfaEndStatu[k] == table1[i][j]) {
					flag = true;
					break;
				}
			}
			if (flag) {
				//����̬
				endStatuTable[i] = re[k][0];
			}
		}
	}
	cout << "��ʼ����dfa\n";
	simplifyDfa(dfa, endStatuTable, trans);
	cout << "�������\n";

	//��ʼ�ϳɴʷ�����������
	FILE* tarFile = fopen("lex.c", "w");

	cout << "д��ͷ�ļ�\n";
	//��д��ͷ�ļ�
	fprintf(tarFile, "#include<stdio.h>\n");
	fprintf(tarFile, "#include<stdlib.h>\n");
	fprintf(tarFile, "#include<string.h>\n");
	fprintf(tarFile, "#include<ctype.h>\n");

	cout << "д����ű�\n";
	//��ӷ��ű�
	fprintf(tarFile, "char symble[]={\n");
	for (int i = 0; i < symbols.size(); i++) {
		fprintf(tarFile, i == symbols.size() - 1 ? "\'%c\'" : "\'%c\',", symbols[i]);
	}
	fprintf(tarFile, "};\n");

	//��ӷ��ű����Ϣ
	fprintf(tarFile, "int lenSym = %d;\n", symbols.size());

	//���ת����
	fprintf(tarFile, "int trans[][%d]={\n", trans[0].size());
	for (int j = 0; j < trans.size(); j++) {
		fprintf(tarFile, "{ ");
		for (int i = 0; i < trans[j].size(); i++) {
			fprintf(tarFile, i == trans[j].size() - 1 ? "%d" : "%d,", trans[j][i]);
		}
		fprintf(tarFile, j == trans.size() - 1 ? " }\n" : " },\n");
	}
	fprintf(tarFile, "};");

	cout << "�����̬��\n";
	//�����̬��
	fprintf(tarFile, "int endStatuTable[%d]={\n", endStatuTable.size());
	for (int i = 0; i < endStatuTable.size(); i++) {
		if (endStatuTable[i] == "none") continue;
		fprintf(tarFile, i == endStatuTable.size() ? "%d" : "%d,", i);
	}
	fprintf(tarFile, "};\n");
	fprintf(tarFile, "int endStatuTableSize=%d;\n", endStatuTable.size());


	cout << "��ӻ�����\n";
	//���ȫ�ֱ���
	//1. ���뻺����
	fprintf(tarFile, "char buf[4096];\n");
	//2. �л�����
	fprintf(tarFile, "int lineTable[4096];\n");
	//3. ����������
	fprintf(tarFile, "int pos = 0;\n");

	cout << "��ӱ�Ҫ���ܺ���\n";
	cout << "getIndexChar\n" << "prepareBuf\n" << "skipSpace\n" << "isEndStatu\n" << "isBorden\n" << "scan\n" << "output\n";
	cout << "main\n";
	//��ӱ�Ҫ�Ĺ��ܺ���
	//1. ��Ӻ��� int getIndexChar��char����
	fprintf(tarFile, "int getIndexChar(char c){\n");
	fprintf(tarFile, "int i = 0;\n for(i=0;i<lenSym;i++){\nif(symble[i]==c){\n return i;\n}\n}\nreturn -1;");
	fprintf(tarFile, "}\n");

	//2. ��Ӻ��� int prepareBuf(char* buf)
	fprintf(tarFile, prepareBufFunctionSource);

	//��ӱ��� int scanPos = 0;
	fprintf(tarFile, "int scanPos = 0;\n");
	//3. ��Ӻ��� void skipSpace() �����ո�
	fprintf(tarFile, skipSpace);
	//4. ��Ӻ��� �жϵ�ǰ״̬�Ƿ�����̬ int isEndStatu(int statu);
	fprintf(tarFile, isEndStatuSrc);
	//���isBorden
	fprintf(tarFile, isBorden);
	//5. ��ɨ�躯������
	fprintf(tarFile, scanSrc);
	//6. ��Ӻ���output
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

	//���������
	fprintf(tarFile, mainSrc);
	fclose(tarFile);
	cout << "�ʷ��������Ѵ����ɹ� �ļ���Ϊlex.c\n";


	return 0;
}


/*

sigle = +|\-|\*|/|=|\(|\)|{|}|:|,|;|<|>|!
dwrod = >=|<=|==|<>
ID = [a-zA-Z][a-zA-Z0-9]*
NUM = num[0-9]*��
RES = import|package|main|show|char|for
*/

/*
sigle = +|\-|\*|/|=|\(|\)|{|}|:|,|;|<|>|!
dwrod = >=|<=|==|!=
ID = [a-zA-Z][a-zA-Z0-9]*
NUM =  [0-9]*
RES = if|else|for|while|do|int|write|read

*/