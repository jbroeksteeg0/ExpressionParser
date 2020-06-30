#include <bits/stdc++.h>
#define IN(a,b) (a.find(b) != a.end())
#define p(a,b) make_pair(a,b)
#define readVec(a) for (int64_t __i = 0; __i<(int64_t)a.size();__i++){cin>>a[__i];}

// jimjam

template<typename T>
void pMin(T &a, T b) {if (b<a){a=b;}}
template<typename T>
void pMax(T &a, T b) {if (b>a){a=b;}}
template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& c);
template<typename A, typename B>
std::ostream& operator<<(std::ostream& os, const std::pair<A,B>& c) {std::cout << "(" << c.first << ", " << c.second << ")";return os;}
template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& c) {
	if (c.size() == 0) {os << "{}"; return os;}
	os << "{" << c[0];
	for (int64_t i = 1; i < (int64_t)c.size(); i++) {os <<", "<<c[i];}
	os << "}";
	return os;
}

#ifdef LOCAL
#define ISDEBUG 1
#else
#define ISDEBUG 0
#endif

using namespace std;

set<char> operations = {'+','-','*','/','^','%'};

map<string, int64_t> priority = {
	{"choose2", 4},
	{"sin", 4},
	{"sqrt", 4},
	{"log2", 4},
	{"^", 3},
	{"*", 2},
	{"/", 2},
	{"%", 2},
	{"+", 1},
	{"-", 1}
};

bool isNumber(string input) {
	bool flag = input != "";
	for (char ch: input) {
		flag &= isdigit(ch);
	}
	return flag;
}

bool isOperation(string input) {
	bool flag = input != "";
	for (char ch: input) {
		flag &= operations.count(ch);
	}
	return flag;
}

enum MathType {
	Number,
	Operation,
	Function,
	Expression
};

struct Component {
	MathType type;
	string value;
	vector<Component> parts = {};
};

struct LinkedNode {
	Component value;
	LinkedNode *before = nullptr, *after = nullptr;

	LinkedNode(Component v) {
		value = v;
	}

	void insertAfter(Component newValue) {
		if (after == nullptr){ // was tail
			after = new LinkedNode(newValue);
			after->before = this;
		} else {
			LinkedNode *temp = after;
			after = new LinkedNode(newValue);
			temp->before = after;
			after->after = temp;
			after->before = this;
		}
	}
	void insertBefore(Component newValue) {
		if (before == nullptr) { // was head
			before = new LinkedNode(newValue);
			before->after = this;
		} else {
			LinkedNode *temp = before;
			before = new LinkedNode(newValue);
			temp->after = before;
			before->before = temp;
			before->after = this;
		}
	}
	void deleteBefore() {
		assert(before != nullptr);

		if (before->isHead()) {
			delete before;
			before = nullptr;
		} else {
			LinkedNode *temp = before;
			temp->before->after = this;
			before = temp->before;

			delete temp;
		}
	}
	void deleteAfter() {
		assert(after != nullptr);
		if (after->isTail()) {
			delete after;
			after = nullptr;
		} else {
			LinkedNode *temp = after;
			after->after->before = this;
			after = temp->after;

			delete temp;
		}
	}

	LinkedNode *getThis() {return this;}
	bool isHead() {return before==nullptr;}
	bool isTail() {return after==nullptr; }
};

vector<Component> parse(string input) {
	vector<Component> res;

	string currentString = "";
	int64_t ind = 0;
	while (ind<input.size()) {
		if (input[ind] == ' ') {ind++; continue;}
		assert(input[ind] != ')'); // should be handled	recursively

		if (input[ind] == '(') {

			if (isNumber(currentString)) { // get rid of old string
				res.push_back({Number, currentString});
			} else if (isOperation(currentString)) { // operation
				res.push_back({Operation, currentString});
			} else if (currentString != "") { // function
				res.push_back({Function, currentString});
			}
			currentString = "";

			ind++;
			int64_t curr = 1;
			string recString = "";
			while (curr != 0){
				if (input[ind] == '(') {
					curr++;
					recString += input[ind];
				} else if (input[ind] == ')'){
					curr--;
					if (curr != 0){
						recString += input[ind];
					}
				} else {
					recString += input[ind];
				}

				if (curr != 0){
					ind++;
				}
			}
			res.push_back({Expression,"",parse(recString)});
		} else if (currentString == "") { // doesn't matter what it is, add
			currentString += input[ind];
		} else if (isNumber(currentString)) {
			if (isdigit(input[ind])) { // another number
				currentString += input[ind];
			} else { // something else
				res.push_back({Number, currentString});
				currentString = input[ind];
			}
		} else if (isOperation(currentString)) { // operations can only be 1 long
			res.push_back({Operation, currentString});
			currentString = input[ind];
		} else { // must be a function
			currentString += input[ind]; // runs until brackets
		}
		ind++;

	}

	assert(currentString == "" || isNumber(currentString));
	if (isNumber(currentString)) {
		res.push_back({Number, currentString});
	}
	return res;
}

int64_t evaluate(vector<Component> input) {
	int64_t maxPriority = 0;
	for (pair<string,int64_t> psi: priority) {
		maxPriority = max(maxPriority, psi.second);
	}

	
	LinkedNode *curr = new LinkedNode{input[0]};
	if (input[0].parts.size()) { // if first value is a bracket expression
		curr->value = Component{Number, to_string(evaluate(input[0].parts))};
	}
	for (int64_t i = 1; i < input.size(); i++) {
		if (input[i].parts.size()) {
			curr->insertAfter({Number, to_string(evaluate(input[i].parts))});
		} else {
			curr->insertAfter(input[i]);
		}
		assert(!curr->isTail());
		curr = curr->after;
	}

	for (int64_t p = maxPriority; p >= 1; p--) { // evaluate things of this priority
		while (!curr->isHead()) {curr=curr->before;}

		while (true) {

			if (curr->value.type == Operation) { // use both sides
				assert(priority.find(curr->value.value) != priority.end());
				string name = curr->value.value;
				assert(priority.find(name) != priority.end());

				if (priority[name] == p) {
					assert(!curr->isHead() && !curr->isTail() && curr->before->value.type == Number && curr->after->value.type == Number);
					int64_t before = stoi(curr->before->value.value);
					int64_t after = stoi(curr->after->value.value);
					int64_t newValue;
					if (name == "+") {newValue = before + after;}
					if (name == "-") {newValue = before - after;}
					if (name == "*") {newValue = before * after;}
					if (name == "/") {newValue = before / after;}
					if (name == "%") {newValue = before % after;}
					if (name == "^") {newValue = before; for (int64_t i = 1; i < after; i++) {newValue *= before;}}

					curr->deleteBefore(); // CAN INVALIDATE TAIL ?
					curr->deleteAfter();
					curr->value = Component{Number, to_string(newValue)};
				}
			} else if (curr->value.type == Function) { // use only to right
				string name = curr->value.value;
				assert(priority.find(name) != priority.end());

				if (priority[name] == p) {
					assert(!curr->isTail() && curr->after->value.type == Number);
				
					string name = curr->value.value;
					int64_t after = stoi(curr->after->value.value);
					int64_t newValue;

					if (name == "choose2") {
						newValue = (after*(after-1))/2;
					}
					curr->deleteAfter();
					curr->value = Component{Number, to_string(newValue)};				
				}
			}

			if (curr->isTail()) {
				break;
			} else {
				curr = curr->after;
			}
		}
	}

	while (!curr->isHead()) {curr = curr->before;}
	assert(curr->isHead() && curr->isTail());

	return stoi(curr->value.value);
}

int main() {
	if (ISDEBUG) {
		freopen("input","r",stdin);
	} else {
		ios::sync_with_stdio(0);
		cin.tie(0);
	}

	string input;
	getline(cin, input);

	vector<Component> tokens = parse(input); // appears to be working
	cout << evaluate(tokens) << "\n"; 

	return 0;
}


