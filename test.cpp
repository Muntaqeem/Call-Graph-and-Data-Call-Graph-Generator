#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <map>
#include <queue>
#include <stack>

using namespace std;

#define MAX 10000
#define pb push_back

#define header "header"
#define macro "macro"
#define statement "statement"
#define start_brace "start_brace"
#define close_brace "close_brace"
#define loop "loop"
#define condition "condition"
#define function "function"





struct node{
	string type;
	string code_segment;
	int pos;
	int parent;
	vector<int>child;
}root;

vector<string> codes;
fstream myFile;
fstream tree_file;
fstream tree_txt;
fstream cg_json;
fstream dcg_json;

int node_idx=1;
vector<node> noded_code;
vector<int> tree[MAX];


string supported_char ="();{}''\"+=-*&|#%^?/<>,";
bool isChar(char x){
	if((x>='A' && x<='Z') || (x>='a' && x<='z')) return true;
	for(int i=0;i<supported_char.size();i++){
		if(x == supported_char[i]) return true;
	}
	return false;
}


bool pureisChar(char x){
	if((x>='A' && x<='Z') || (x>='a' && x<='z')) return true;
    return false;
}
bool starts_with(string x,string with){
	if(x.size()>with.size()) return false;

	for(int i=0;i<x.size();i++){
		if(x[i]!=with[i]) return false;
	}
	return true;

}



void store()
{
	string line;

    if (myFile.is_open())
    {
   	    while ( getline (myFile,line) )
    	{
      		// cout << line << '\n';
    		if(line.size()<1) continue;
      		codes.push_back(line + "\n");
    	}

    }

}


void printVector()
{
	for(int i=0; i<codes.size(); i++)
    {

    	cout << codes.at(i)<<endl;;
    }
}

string cutFunctionName(string str){
    string func ="";
    for(int i=0;i<str.size();i++){
        if(str[i]==' '){
            func="";
        }
        else if(str[i]=='('){
            return func;
        }
        else{
            func+=str[i];
        }
    }

    return func;
}





node create_node(string line){

	node temp;
	temp.pos = node_idx++;
	temp.code_segment=line;

	if(starts_with("#include",line)==true){
		temp.type="header";
	}
	else if(starts_with("#define",line)==true){
		temp.type="macro";
	}
	else if(line[line.length()-1]==';'){
		temp.type = "statement";
	}
	else if(line.length()==1){
		if(line[0]=='{'){
			temp.type = "start_brace";
		}
		else temp.type="close_brace";
	}
	else if(line[line.length()-1]==')' || line[line.length()-1]=='{' || line[line.length()-1]=='e'){
		if(starts_with("for",line)==true || starts_with("while",line)==true || starts_with("do",line)==true){
			temp.type = "loop";
		}
		else if(starts_with("if",line)==true || starts_with("else",line)==true){
			temp.type= "condition";
		}
		else{
			temp.type = "function";
		}
	}
	return temp;

}


void dfs(int cur){
	if(tree[cur].size()>0)
		cout<<cur<<" -> ";
	
	for(int i=0;i<tree[cur].size();i++){
		cout<<tree[cur][i]<<' ';
	}
	
	if(tree[cur].size()>0)
		cout<<endl;
	
	for(int i=0;i<tree[cur].size();i++){
		if(tree[cur][i]!=cur) dfs(tree[cur][i]);
	}

}



void generate_tree(){

    node_idx=1;
	int cur = 0;

    node dummy;
    dummy.parent=11111;
    noded_code.pb(dummy);

	for(int i=0;i<codes.size();i++){
		string line = codes[i];
		node temp = create_node(line);
		noded_code.pb(temp);
		cout<<line<<" -> "<<noded_code[i+1].pos<<' '<<noded_code[i+1].type<<  " " /*<< noded_code[i].code_segment */ ;

		if(temp.type == "loop" || temp.type == "condition" || temp.type =="function"){
			noded_code[i+1].parent = cur;
			tree[cur].pb(temp.pos);
			cur = temp.pos;
		}
		else if(temp.type == "close_brace"){
			noded_code[i+1].parent = cur;
            // cout<<"-- "<<i<<' '<<cur<<' '<<noded_code[cur].parent<<" --";
			cur = noded_code[cur].parent;
		}
		else{
			tree[cur].pb(temp.pos);
			noded_code[i+1].parent=cur;
            // cout<<"chk 1 : "<<i<<' '<<cur<<endl;
		}
		cout << noded_code[i+1].parent << " " << endl;
  //       if(noded_code.size()>=9){
  //           cout<<"Check ";
  //           cout<<' '<<noded_code[9].parent<<endl;
  //       }
        // if(noded_code[i].parent == noded_code)
        // {

        // }


	}
	cout<<endl;
	cout<<"Generated tree: "<<endl;
	dfs(0);



}



void trim()
{
	for(int j=0; j<codes.size(); j++)
    {

    	string line;
    	line = codes[j];

    	for(int i=0; i<line.size(); i++)
    	{
    		if(isChar(line[i])==true && i!=0){
    			line.erase(0,i);
    			break;
    		}
    		else if(isChar(line[i])==true) {
				break;
			}
    	}
    	for(int i=line.length()-1;i>=0;i--){
    		if(isChar(line[i])==false){
    			line.erase(i,1);
    		}
    		if(isChar(line[i])==true) break;
    	}
    	codes[j]=line;
    }
    // printVector();
}


vector<string>functions;
map<string,bool> functions_map;
map<string,string> functions_name;

void extract_functions(){
    for(int i=0;i<tree[0].size();i++){
        node temp = noded_code[tree[0][i]];
        string st="";
        if(temp.type==function){
            cout<<temp.code_segment<<endl;
            if(temp.code_segment[temp.code_segment.length()-1]!=')'){
                temp.code_segment.erase(temp.code_segment.length()-1,1);
            }
            bool flag = false;
            for(int j=0;temp.code_segment[j]!='(';j++){
                if(temp.code_segment[j]==' ') {
                    flag=true;
                    continue;
                }
                if(flag) st+=temp.code_segment[j];
            }
            functions.pb(st);
            functions_map[st]=true;
            functions_name[temp.code_segment]=st;
        }
    }
    cout<<endl;
    cout<<"Functions:"<<endl;
    for(int i=0;i<functions.size();i++){
        cout<<functions[i]<<endl;
    }
}




vector<string> global_variable;

void clean_global_variable(){
    for(int i=0;i<global_variable.size();i++){
        string temp = global_variable[i];

        if(temp[0]=='*') global_variable[i].erase(0,1);
        temp = global_variable[i];

        if(temp[temp.length()-1]==']'){
            for(int j=temp.length()-1;;j--){
                if(temp[j]=='['){
                    global_variable[i].erase(j,temp.length()-j);
                    break;
                }
            }
        }
    }
}



void parse_global_variable(string line){
    for(int i=0;i<line.length();i++){
        if(line[i]==' '){
            line.erase(0,i+1);

            break;
        }
    }
    for(int i=0;i<line.length();i++){
        if(line[i]==' ' || line[i]==';'){
            line.erase(line.begin()+i);
        }
    }

    string temp="";
    for(int i=0;i<line.length();i++){
        if(line[i]==','){
            global_variable.pb(temp);
            temp="";
            continue;
        }
        temp+=line[i];
    }
    if(temp!="") global_variable.pb(temp);

    clean_global_variable();
}


void extract_global_variable(){
    cout<<"started extraction"<<endl;
	for(int i=0;i<tree[0].size();i++){
		int pos = tree[0][i];
		node temp = noded_code[pos];
//        cout<<temp.type<< ' '<<temp.code_segment<<endl;
		if(temp.type == statement && temp.code_segment[temp.code_segment.length()-2]!=')'){
			parse_global_variable(temp.code_segment);
		}
	}

	cout<<endl;
	cout<<"Global variable"<<endl;
	for(int i=0;i<global_variable.size();i++){
		cout<<global_variable[i]<<endl;
	}
}

vector<int> fun_mat[MAX];

void function_matrix()
{
    cout<<"Function matrix started"<<endl;
    node pushing_top;
    for(int i=0;i<tree[0].size();i++){
        node temp = noded_code[tree[0][i]];
        if(temp.type==function){
            // cout<<temp.code_segment<<endl;
            queue<node> q;
            q.push(temp);
            while(!q.empty()){
                node top=q.front();
                if(top.type==function){
                    pushing_top=top;
                }
                q.pop();
//                cout<<top.code_segment<<"-> ";
                for(int j=0;j<tree[top.pos].size();j++){
                    node child = noded_code[tree[top.pos][j]];
                    // cout<<child.code_segment<<' '<<child.type<<',';
                    if(child.type==statement && child.code_segment[child.code_segment.length()-2]==')'){
                        cout<<child.code_segment<<endl;
                        string x="";
                        for(int k=0;k<child.code_segment.length();k++){
                            if(child.code_segment[k]=='('){

                                if(functions_map.find(x)!=functions_map.end()){
                                    // cout<<top.code_segment<<' '<<top.type<<" paici"<<endl;
                                    fun_mat[pushing_top.pos].pb(child.pos);
                                    break;
                               }
                            }
                            x+=child.code_segment[k];
                        }


                    }
                    q.push(child);
                }
               // cout<<endl;
            }
        }
    }
    cout<<endl;
    for(int i = 0 ;i<tree[0].size();i++){
        node temp = noded_code[tree[0][i]];

        if(temp.type==function){
            cout<<fun_mat[temp.pos].size()<<' '<<temp.code_segment<<" -> ";
            for(int j=0;j<fun_mat[temp.pos].size();j++){
                cout<<noded_code[fun_mat[temp.pos][j]].code_segment <<' ';
            }
            cout<<endl;
        }
    }
}


map<string,vector<int> > variable_read;
map<string, vector<int> > variable_write;

void variable_matrix(string var)
{
    cout<<"variable matrix generation started..."<<endl;


    for(int i=0;i<tree[0].size();i++){
        node temp = noded_code[tree[0][i]];
        if(temp.type==function){
            queue<node> q;

            q.push(temp);
            while(!q.empty()){
                node top = q.front();
//                cout<<top.code_segment<<endl;
                q.pop();
                bool flag= false;
                for(int j=0;j<tree[top.pos].size();j++){
                    node child = noded_code[tree[top.pos][j]];
                    q.push(child);
                    if(child.type==statement){
                        int idx = child.code_segment.find(var);
                        if(idx==0){
                            if(pureisChar(child.code_segment[var.length()])==false){
                                variable_write[var].pb(tree[0][i]);
                                flag=true;
                                break;
                            }
                        }
                        idx = child.code_segment.find(var);
                        string x = child.code_segment;
                        while(idx<MAX && idx > -1){
                            if(pureisChar(child.code_segment[idx+var.length()])==false && pureisChar(child.code_segment[idx-1])==false){
                                variable_read[var].pb(tree[0][i]);
                                flag = true;
                                break;
                            }
                            x = x.substr(idx+var.length());
                            idx = x.find(var);
                            cout<<idx<<endl;
                        }
                    }
                }
                if(flag) break;
            }
        }
    }


}


void generate_variable_mat(){
    for(int i=0;i<global_variable.size();i++){
        variable_matrix(global_variable[i]);
    }
    cout<<endl;
    cout<<"read"<<endl;
    for(int i=0;i<global_variable.size();i++){
        cout<<global_variable[i]<<" -> ";
        for(int j=0;j<variable_read[global_variable[i]].size();j++){
            cout<<noded_code[variable_read[global_variable[i]][j]].code_segment<<' ';
        }
        cout<<endl;
    }
    cout<<endl;
    cout<<"Write"<<endl;
    for(int i=0;i<global_variable.size();i++){
        cout<<global_variable[i]<<" -> ";
        for(int j=0;j<variable_write[global_variable[i]].size();j++){
            cout<<noded_code[variable_write[global_variable[i]][j]].code_segment<<' ';
        }
        cout<<endl;
    }
}


void generate_tree_json_file()
{
	tree_file.open("tree.json");

    tree_file << "";

	tree_file << "{" << endl << "\"nodes\" : [";


	for(int i=1; i<noded_code.size(); i++)
	{
		tree_file << "[\"" << i << "\", \"" << noded_code[i].parent << "\"]";
		if (i < noded_code.size()-1)
		{
			tree_file << "," << endl;
		}
		
	}

	tree_file << "]"<< endl <<"}";

	tree_file.close();
}

void generate_tree_txt_file()
{
	tree_txt.open("tree.txt");
    tree_txt << "";
	for(int i=0; i<noded_code.size();i++)
	{
        if(i<noded_code.size()-1)
        {    
		  tree_txt << noded_code[i].code_segment << "$$";
        }
        else
        {
            tree_txt << noded_code[i].code_segment;
        }
	}
	tree_txt.close();
}

void generate_call_graph_json_file()
{
    cg_json.open("cg.json");
    cg_json << "";

    cg_json << "{" << endl << "\"nodes\" : [" << endl;

    int temp1,temp2;

    for(int i=0; i<functions.size(); i++)
    {
        cg_json << "{" << endl;

        cg_json << "\"id\":" << i << ",";
        cg_json << "\"caption\": " << "\"" <<functions[i] << "\"" << endl;

        cg_json << "}";

        if(i<functions.size()-1)
        {
            cg_json << ","<< endl;
        }
        else
            cg_json << endl;

    }

    cg_json << "]," << endl;

    cg_json << "\"edges\" : [" << endl;

    for(int i = 0 ;i<tree[0].size();i++){
        node temp = noded_code[tree[0][i]];

        if(temp.type==function){
            int source = -2;
            for (int k =0; k< functions.size(); k++)
            {
                if(functions[k]==cutFunctionName(temp.code_segment))
                {
                    source = k;
                    temp1 = source;
                }
            }
            for(int j=0;j<fun_mat[temp.pos].size();j++){

                cg_json << "{" << endl;

                cg_json << "\"source\":" << source << ",";
                int target = -2;

                for (int k =0; k< functions.size(); k++)
                {
                    if(functions[k]==cutFunctionName (noded_code[fun_mat[temp.pos][j]].code_segment))
                    {
                        target = k;
                        temp2 = target;
                    }
                }
                cg_json << "\"target\": " << target << endl;



                // cout << "j er value " << fun_mat[temp.pos].size() << endl;
                cg_json << "}," << endl;
                cout<<cutFunctionName (noded_code[fun_mat[temp.pos][j]].code_segment) <<' ';
            }
            
            cout<<endl;
        }
    }

    cg_json<< "{" << endl << "\"source\": " << temp1 << ",";
            cg_json << "\"target\": " << temp2 << endl << "}" << endl;


    cg_json << "]" << endl;





    cg_json << "}";

    cg_json.close();

}


void generate_data_call_graph_json_file()
{
    dcg_json.open("dcg.json");
    dcg_json << "";

    dcg_json << "{";

    dcg_json << endl << "\"nodes\" : [" << endl;

    int id = 0;
    map<string,int> id_map;

    for(int i=0; i<functions.size(); i++)
    {
        dcg_json << "{" << endl;

        dcg_json << "\"id\":" << id << ",";
        dcg_json << "\"nodeType\":" << "\"Function\"" << ",";

        dcg_json << "\"caption\": " << "\"" <<functions[i] << "\"" << endl;

        dcg_json << "}";

        dcg_json << ","<< endl;
        id_map[functions[i]] = id;

        id++;

    }

    for (int i= 0; i < global_variable.size(); i++)
    {
        dcg_json << "{" << endl;

        dcg_json << "\"id\":" << id << ",";
        dcg_json << "\"nodeType\":" << "\"Variable\"" << ",";
        dcg_json << "\"caption\": " << "\"" <<global_variable[i] << "\"" << endl;

        dcg_json << "}";
        id_map[global_variable[i]] = id;
        id++;

        if(i<global_variable.size()-1)
        {
            dcg_json << "," << endl;
        }
        else
            dcg_json << endl;

    }

    dcg_json << "]," << endl;

    //edges started *************************************************
    dcg_json << "\"edges\" : [" << endl;
    int temp1,temp2;

    //call edges started

    for(int i = 0 ;i<tree[0].size();i++){
        node temp = noded_code[tree[0][i]];

        if(temp.type==function){
            int source = -2;
            for (int k =0; k< functions.size(); k++)
            {
                if(functions[k]==cutFunctionName(temp.code_segment))
                {
                    source = k;
                    temp1 = source;
                }
            }
            for(int j=0;j<fun_mat[temp.pos].size();j++){

                dcg_json << "{" << endl;

                dcg_json << "\"source\":" << source << ",";
                int target = -2;

                for (int k =0; k< functions.size(); k++)
                {
                    if(functions[k]==cutFunctionName (noded_code[fun_mat[temp.pos][j]].code_segment))
                    {
                        target = k;
                        temp2 = target;
                    }
                }
                dcg_json << "\"target\": " << target << ",";
                dcg_json << "\"edgeType\": \"CALL\"" << endl;



                // cout << "j er value " << fun_mat[temp.pos].size() << endl;
                dcg_json << "}," << endl;
                //cout<<cutFunctionName (noded_code[fun_mat[temp.pos][j]].code_segment) <<' ';
            }
            
            cout<<endl;
        }
    }


    //call edges ended

    //read edeges started

    for(int i=0;i<global_variable.size();i++){

        int source=-2;
        cout<<global_variable[i]<<" -> ";
        for(int j=0;j<variable_read[global_variable[i]].size();j++)
        {

            dcg_json << "{" << endl;

            dcg_json << "\"source\":" << id_map[cutFunctionName(noded_code[variable_read[global_variable[i]][j]].code_segment)] << ",";
            
            dcg_json << "\"target\": " << id_map[global_variable[i]] << ",";
            dcg_json << "\"edgeType\": \"READ\"" << endl;

            dcg_json << "}," << endl;
        }
    }

    //read edges ended

    //write edges started

    for(int i=0;i<global_variable.size();i++){

        int source=-2;
        cout<<global_variable[i]<<" -> ";
        for(int j=0;j<variable_write[global_variable[i]].size();j++)
        {

            dcg_json << "{" << endl;

            dcg_json << "\"source\":" << id_map[cutFunctionName(noded_code[variable_write[global_variable[i]][j]].code_segment)] << ",";
            
            dcg_json << "\"target\": " << id_map[global_variable[i]] << ",";
            dcg_json << "\"edgeType\": \"WRITE\"";

            dcg_json << "}," << endl;

            //cout<<noded_code[variable_read[global_variable[i]][j]].code_segment<<' ';
        }
        //cout<<endl;
    }


    //write edges ended


    dcg_json << "{" << endl << "\"source\": " << temp1 << ",";
    dcg_json << "\"target\": " << temp2 << ",";
    dcg_json << "\"edgeType\": \"CALL\"" << endl << "}";


    dcg_json << "]" << endl;



    dcg_json << "}";

    dcg_json.close();

}

int main()
{
    string ch;
    ch = "ForTestCode.c";
 
    int len = ch.length();
 
    if(len>2){
        if(ch[len-2]=='.' && ch[len-1]=='c'){
            cout<<"OK, File type : C"<<endl;
        }
        else{
            cout<<"Invalid extension!"<<endl;
        }
    }
    else{
        cout<<"Invalid file"<<endl;
    }

	myFile.open(ch);

	store();
    // printVector();
    trim();
    generate_tree();
    generate_tree_txt_file();
    extract_global_variable();

    extract_functions();
    function_matrix();
    generate_variable_mat();
    generate_tree_json_file();

    generate_call_graph_json_file();
    generate_data_call_graph_json_file();

    
    myFile.close();

    return 0;

}
