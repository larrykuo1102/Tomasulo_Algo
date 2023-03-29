

#include <iostream>
# include <string>
# include <vector>
# include <map>
# include <queue>

using namespace std ;

map<string, string> Operation ;
map<string,int> RF;
map<string,string> RAT ;
vector<vector<string> > Add_RS, MUL_RS ;
vector<string> ALU_ADD, ALU_MUL ; // Perform buffer 正在處理的
vector<string> next_ALU_ADD, next_ALU_MUL ; // 下一個要處理的
int gALU_ADD_cycle = 0, gALU_MUL_cycle = 0 ; // 現在正在處理的需要跑的cycle
vector<string> ADD_result, MUL_result ; // 處理完的結果

queue<vector<string> > Instruction ;



//vector<vector<string>> Instruction ;
int gCurrent_inst = 0 ;

int gCurrent_cycle = 1 ;

bool isNumber( string str )
{
    for ( auto c : str) {
        if ( isdigit(c) == 0) return false;
    }
    return true;
}

string read_string() {
    
    string a_string ;
    char onechar = cin.peek() ;
    while( cin.peek() != '\n' && cin.peek() != -1 ) {
        onechar = cin.peek() ;
        if ( onechar == ' ' || onechar == '\n' || onechar == ',' || onechar == ';' || onechar == '\t' ){
            break;
        } // if
        
        a_string += cin.get() ;
    } // while

    return a_string ;
} // read_string()

void skip_comment() {
    while( cin.peek() != '\n' && cin.peek() != -1 ) {
        cin.get() ;
    } // while
} // skip_comment

vector<string> GetToken() {
    char onechar ;
    string test_string ;
    vector<string> tempLine ;
    while( cin.peek() != -1 ) {
        onechar = cin.peek() ;
        if ( onechar == ' ' || onechar == '\n' || onechar == ',' || onechar == ';' || onechar == '\t' ) {
            if ( onechar ==  ';' )
                skip_comment() ;
            else if ( onechar == '\n' ) {
                if ( tempLine.size() != 0 )
                    Instruction.push( tempLine ) ;
                tempLine.clear() ;
                cin.get() ;
            } // else if
            else
                cin.get() ;
        } // if
        else {
            test_string = read_string() ;
            tempLine.push_back( test_string ) ;
        } // else
    } // while
    
    return tempLine ;
} // GetToken



void Ouput_RS() {
//    vector<string> temp ;
//    temp.push_back("-") ;
//    temp.push_back("12") ;
//    temp.push_back("3") ;
//    Add_RS[0] = temp ;
    cout << "\t_ RS ____________________" << endl ;
    cout << "RS1 |\t" ;
    if ( Add_RS[0].size() != 0 ) {
        cout << Add_RS[0][0] << "\t|\t" << Add_RS[0][1] << "\t|\t" << Add_RS[0][2] << "\t|" << endl ;
    } // if
    else {
        cout << "\t|\t\t|\t\t|" << endl ;
    } // else
    cout << "RS2 |\t" ;
    if ( Add_RS[1].size() != 0 ) {
        cout << Add_RS[1][0] << "\t|\t" << Add_RS[1][1] << "\t|\t" << Add_RS[1][2] << "\t|" << endl ;
    } // if
    else {
        cout << "\t|\t\t|\t\t|" << endl ;
    } // else
    cout << "RS3 |\t" ;
    if ( Add_RS[2].size() != 0 ) {
        cout << Add_RS[2][0] << "\t|\t" << Add_RS[2][1] << "\t|\t" << Add_RS[2][2] << "\t|" << endl ;
    } // if
    else {
        cout << "\t|\t\t|\t\t|" << endl ;
    } // else
    
    cout << "\t-------------------------" << endl ;
    cout << "BUFFER: " ;
    if ( ALU_ADD.size()== 0 )
        cout << "empty" << endl ;
    else {
        for( int i = 0 ; i < ALU_ADD.size() ; i ++ ) {
            cout << ALU_ADD[i] << " " ;
        } // for
        cout << endl << endl ;
    } // else
    cout << "\t-------------------------" << endl ;
    cout << "RS4 |\t" ;
    if ( MUL_RS[0].size() != 0 ) {
        cout << MUL_RS[0][0] << "\t|\t" << MUL_RS[0][1] << "\t|\t" << MUL_RS[0][2] << "\t|" << endl ;
    } // if
    else {
        cout << "\t|\t\t|\t\t|" << endl ;
    } // else
    cout << "RS5 |\t" ;
    if ( MUL_RS[1].size() != 0 ) {
        cout << MUL_RS[1][0] << "\t|\t" << MUL_RS[1][1] << "\t|\t" << MUL_RS[1][2] << "\t|" << endl ;
    } // if
    else {
        cout << "\t|\t\t|\t\t|" << endl ;
    } // else
    
    cout << "\t-------------------------" << endl ;
    cout << "BUFFER: " ;
    if ( ALU_MUL.size()== 0 )
        cout << "empty" << endl ;
    else {
        for( int i = 0 ; i < ALU_MUL.size() ; i ++ ) {
            cout << ALU_MUL[i] << " " ;
        } // for
        cout << endl ;
    } // else
} // Ouput_RS

void Output_RF() {
    cout << "\t_ RF __" << endl ;
    cout << "F1\t|\t" << RF["F1"] << "  |" << endl ;
    cout << "F2\t|\t" << RF["F2"] << "  |" << endl ;
    cout << "F3\t|\t" << RF["F3"] << "  |" << endl ;
    cout << "F4\t|\t" << RF["F4"] << "  |" << endl ;
    cout << "F5\t|\t" << RF["F5"] << "  |" << endl ;
    cout << "\t--------" << endl << endl ;
} // Output_RF

void Output_RAT() {
    cout << "\t_ RAT __" << endl ;
    cout << "F1\t|\t" ;
//    RAT["F1"] = "RS1" ;
    if ( RAT["F1"] != "" )
        cout << RAT["F1"] ;

    cout <<"\t|" << endl ;
    cout << "F2\t|\t" ;
    if ( RAT["F2"] != "" )
        cout << RAT["F2"] ;
    cout <<"\t|" << endl ;
    cout << "F3\t|\t" ;
    if ( RAT["F3"] != "" )
        cout << RAT["F3"] ;
    cout <<"\t|" << endl ;
    cout << "F4\t|\t" ;
    if ( RAT["F4"] != "" )
        cout << RAT["F4"] ;
    cout <<"\t|" << endl ;
    cout << "F5\t|\t" ;
    if ( RAT["F5"] != "" )
        cout << RAT["F5"] ;
    cout <<"\t|" << endl ;
    cout << "\t--------" << endl << endl ;
} // Output_RAT

void Output_ALL() {
    cout << "Cycle: " << gCurrent_cycle << endl ;
    Output_RF() ;
    Output_RAT() ;
    Ouput_RS() ;
} // Output_ALL

void Init_operation() {
    Operation.insert(pair<string, string>("ADDI","+")) ;
    Operation.insert(pair<string, string>("ADD","+")) ;
    Operation.insert(pair<string, string>("SUB","-")) ;
    Operation.insert(pair<string, string>("MUL","*")) ;
    Operation.insert(pair<string, string>("DIV","/")) ;
} // Init_operation


void Init() {
    vector<string> temp ;
    for( int i = 0 ; i < 3 ; i ++ ) {
        Add_RS.push_back(temp) ;
    } // for
    for( int i = 0 ; i < 2 ; i ++ ) {
        MUL_RS.push_back(temp) ;
    } // for
    
    RF["F1"] = 0 ;
    RF["F2"] = 2 ;
    RF["F3"] = 4 ;
    RF["F4"] = 6 ;
    RF["F5"] = 8 ;
    
    RAT["F1"] = "none" ;
    RAT["F2"] = "none" ;
    RAT["F3"] = "none" ;
    RAT["F4"] = "none" ;
    RAT["F5"] = "none" ;
    
    Init_operation() ;
} // Init_RF

bool find_RAT( string reg ) {
    if ( RAT[reg] != "none" ) {
        return true ;
    } // if
    return false ;
} // find_RAT

void Insert_RS( vector<string> temp, string type, int num ) { // + F1 F2 (cycle) RS1
    vector<string> insert ;
    insert.push_back( Operation[temp[0]] ) ;
    // 加入參數 有三種情況
    // 1. 有可能會在RAT裡面 2. 在RF裡面的直 3. 常數
    if ( RAT[temp[2]] != "none" ) // none 代表沒有在RAT
        insert.push_back(  RAT[temp[2]]  ) ;
    else if ( RF.find( temp[2] ) != RF.end() ) // 如果是常數的話 會return end()的iterator
        insert.push_back( to_string(RF[temp[2]])  ) ;
    else
        insert.push_back( temp[2] ) ;
    if ( RAT.find(temp[3]) != RAT.end() && RAT[temp[3]] != "none" ) // none 代表沒有在RAT
        insert.push_back(  RAT[temp[3]]  ) ;
    else if ( RF.find( temp[3] ) != RF.end() ) // 如果是常數的話 會return end()的iterator
        insert.push_back( to_string(RF[temp[3]])  ) ;
    else
        insert.push_back( temp[3] ) ;
 
    if ( type == "ADD" ) { // 放入 RAT
        insert.push_back( to_string(gCurrent_cycle) ) ; // 在[3]的地方 放入cycle
        insert.push_back( "RS" + to_string( num + 1 ) ) ; // 在[4]的地方 放入"RS1"
        Add_RS[num] = insert ;
        RAT[temp[1]] = "RS" + to_string( num + 1 ) ;
    } // if
    else {
        insert.push_back( to_string(gCurrent_cycle) ) ;
        insert.push_back( "RS" + to_string( num + 4 ) ) ;
        MUL_RS[num] = insert ;
        RAT[temp[1]] = "RS" + to_string( num + 4 ) ;
    } // else
    
//    Output_ALL() ;
} // Insert_RS

void Issue() { // **********  + 1 2 (currentcycle) "RS1"   ************
    //每次一cycle
    // 先看下一個instruction是 ADD/SUB or MUL/DIV
    // 看對應的RS有沒有空位 有得話加進去
    // 有的話放進去 -> 找出register對應的value 或是 RAT 裡的 RS
    // 在RAT裡面放入對應的RS
    // cycle ++
    vector<string> temp ;

    temp = Instruction.front() ;
    if ( Operation[temp[0]] == "+" || Operation[temp[0]] == "-" ) {
        if ( Add_RS[0].empty()  ) { // 確認ADD的RS第 1 格是不是空的
            Instruction.pop() ;
            Insert_RS( temp, "ADD", 0 ) ; // 將指令加入到RS裡面
        } // if
        else if ( Add_RS[1].empty() ) { // 確認ADD的RS第 2 格是不是空的
            Instruction.pop() ;
            Insert_RS( temp, "ADD", 1 ) ;
        } // else if
        else if ( Add_RS[2].empty() ) { // 確認ADD的RS第 3 格是不是空的
            Instruction.pop() ;
            Insert_RS( temp, "ADD", 2 ) ;
            
        } // else if
        else {
            
        } // else
    } // if
    else {
        if ( MUL_RS[0].empty() ) { //  確認MUL的RS第 1 格是不是空的
            Instruction.pop() ;
            Insert_RS( temp, "MUL", 0 ) ;
        } // if
        else if (  MUL_RS[1].empty() ) { // 確認MUL的RS第 2 格是不是空的
            Instruction.pop() ;
            Insert_RS( temp, "MUL", 1 ) ;
        } // else if
        else {
            
        } // else
    } // else
    
    
} // Issue

vector<int> Check_RS( string temp ) {
    vector<int> check ;
    int flag = 1 ;
    
    if ( temp == "ADD") {
        for ( int i = 0 ; i < 3 ; i ++ ) {
            if ( Add_RS[i].size() != 0 ) {
                for( int j = 1 ; j < 3 ; j ++ ) {
                    if ( ! isNumber(  Add_RS[i][j] ) ) { // 如果 Add_RS[0][1] 裡面是數字的話
                        flag = 0 ;
                    } //
                    
                } //
            } // if
            else if ( Add_RS[i].size() == 0 ) {
                flag = 0 ;
            } // else if
            if ( Add_RS[i].size() != 0 && ALU_ADD.size() > 2 &&  Add_RS[i][3] == ALU_ADD[3]  ) {
                flag = 0 ;
            } // else if
            
            check.push_back( flag ) ;
            flag = 1 ;
        } // for
    } // if
    else {
        for ( int i = 0 ; i < 2 ; i ++ ) {
            
            if ( MUL_RS[i].size() != 0 ) {
                for( int j = 1 ; j < 3 ; j ++ ) {
                    if ( ! isNumber(  MUL_RS[i][j] ) ) { // 如果 Add_RS[0][1] 裡面是數字的話
                        cout << MUL_RS[i][j] ;
                        flag = 0 ;
                    } //
                } //
            } // if
            else if ( MUL_RS[i].size() == 0 ) {
                flag = 0 ;
            } // else if
            if ( MUL_RS[i].size() != 0 && ALU_MUL.size() > 2 && MUL_RS[i][3] == ALU_MUL[3]  ) {
                flag = 0 ;
            } // else if
            
            check.push_back( flag ) ;
            flag = 1 ;
        } // for
    } // else
    
    
    return check ;
} // Check_RS

int find_oldest_RS( vector<vector<string> > RS, vector<int> check ) { // 需要再檢查一次
    if ( count(check.begin(), check.end(), 1 ) > 1 ) {
//        cout << count(check.begin(), check.end(), 1 ) << "test" << gCurrent_cycle << endl ;
        int oldest = 5 ; // RS[oldest]
        if ( !RS[0].empty() && !RS[1].empty() ) {
            if (  RS[0][3] < RS[1][3]  ) { // 越小越old
                if ( oldest == 5 )
                    oldest = 0 ;
                else if ( oldest != 5 && RS[0][3] < RS[oldest][3] )
                    oldest = 0 ;
            } // if
            else { // RS[0][3] > RS[1][3]
                if ( oldest == 5 )
                    oldest = 1 ;
                else if ( oldest != 5 && RS[1][3] < RS[oldest][3] )
                    oldest = 1 ;
            } // else
        } // if
        if ( RS.size() >= 3 && ! RS[0].empty() && ! RS[2].empty() ) {
            if (  RS[0][3] < RS[2][3]  ) { // 越小越old
                if ( oldest == 5 )
                    oldest = 0 ;
                else if ( oldest != 5 && RS[0][3] < RS[oldest][3] )
                    oldest = 0 ;
            } // if
            else {
                if ( oldest == 5 )
                    oldest = 2 ;
                else if ( oldest != 5 && RS[2][3] < RS[oldest][3] )
                    oldest = 2 ;
            } // else
        } // else if
        if ( RS.size() >= 3 && ! RS[1].empty() && !RS[2].empty()  ) {
            if (  RS[1][3] < RS[2][3]  ) { // 越小越old
                if ( oldest == 5 )
                    oldest = 1 ;
                else if ( oldest != 5 && RS[2][3] < RS[oldest][3] )
                    oldest = 1 ;
            } // if
            else {
                if ( oldest == 5 )
                    oldest = 2 ;
                else if ( oldest != 5 && RS[2][3] < RS[oldest][3] )
                    oldest = 2 ;
            } // else
        } // else if
        
        return oldest ;
    } // if
    else {
        for ( int i = 0 ; i < check.size() ; i ++ ) {
            if ( check[i] == 1 )
                return i ;
        } //
        return 0 ;
    } //
    
} // find_oldest_RS

void Dispatch( ) {
    // 如果perform buffer的alu 裡面是空的(or 倒數0)話 就要把RS 放到buffer裡面執行
    // 如果是空的(or 倒數0)-> 檢查＆dispatch、 如果不是空的-> 跳過此步驟
    // 檢查 有沒有合格的RS是可以dispatch的
    // 合格要檢查 RS中 [0][1]裡面的value 是不是數字 而不能是RS's name
    // (必須要檢查所有的) 如果有同時合格的話就必須要使用oldest first的方法
    vector<int> check_ADD, check_MUL ; // 1 0
    int process_RS_ADD = -1, process_RS_MUL = -1 ;
    
    // ALU buffer
    check_ADD = Check_RS( "ADD" ) ; // 檢查....
    check_MUL = Check_RS( "MUL" ) ;
    
    
    if ( ALU_ADD.empty() || gALU_ADD_cycle == 0  ) { // 對於 ADD ALU
        if ( count(check_ADD.begin(), check_ADD.end(), 1 ) != 0 ) { // 如果有至少一個是準備好可以進入buffer的
            
            process_RS_ADD = find_oldest_RS( Add_RS, check_ADD ) ; // 尋找最舊的rs return的值是vector的參數
            
            vector<string> temp = Add_RS[process_RS_ADD] ;
            next_ALU_ADD = temp ; // 將要放入的buffer的 先放進暫存
        } // if
    } // if
    
    // MUL buffer
    if ( ALU_MUL.empty() || gALU_MUL_cycle == 0  ) { // 對於 MUL ALU
        if ( count(check_MUL.begin(), check_MUL.end(), 1 ) != 0 ) { // 如果有至少一個是準備好可以進入buffer的
            
            process_RS_MUL = find_oldest_RS( MUL_RS, check_MUL ) ;
            vector<string> temp = MUL_RS[process_RS_MUL] ;
            next_ALU_MUL = temp ; // + 1 2 (currentcycle) "RS1"
        } // if
    } // if
    
    // Dispatch
} // Dispatch

void Calculate() { // and Write Rigister
    int add = 2, mul = 5, div = 10 ;
    string temp ;
    if ( gALU_ADD_cycle != 0 ) { // 還沒有計算完
        gALU_ADD_cycle -- ;
    } // if
    else if ( ! ALU_ADD.empty() && gALU_ADD_cycle == 0 ) { // 如果計算完了
        if ( ALU_ADD[0] == "-" ) { // 如果計算完了 把結果加入 ADD_result 給write result 使用
            ADD_result.push_back( ALU_ADD[4] ) ; // RS1 1(result)
            ADD_result.push_back( to_string( stoi( ALU_ADD[1] ) - stoi( ALU_ADD[2] ) ) ) ;
            string temp(1,ALU_ADD[4][2]) ;
            Add_RS[stoi(temp)-1].clear() ;
            ALU_ADD.clear() ;
        } // if
        else if ( ALU_ADD[0] == "+" ) {
            ADD_result.push_back( ALU_ADD[4] ) ; // RSX result
            ADD_result.push_back( to_string( stoi( ALU_ADD[1] ) + stoi( ALU_ADD[2] ) ) ) ;
            string temp(1,ALU_ADD[4][2]) ;
            Add_RS[stoi(temp)-1].clear() ;
            ALU_ADD.clear() ;
        } // if
    
    } // else if
    if ( ALU_ADD.empty() && ! next_ALU_ADD.empty() ) { // 有下一個指令要進來計算
        gALU_ADD_cycle = add - 1 ;
        ALU_ADD = next_ALU_ADD ;
        next_ALU_ADD.clear() ;
    } // if
    
    if ( gALU_MUL_cycle != 0 ) {
        gALU_MUL_cycle -- ;
    } // if
    else if ( ! ALU_MUL.empty() &&  gALU_MUL_cycle == 0 ) { // 如果現在buffer裡面是有東西的 但是cycle=0 -> 做完
        
        if ( ALU_MUL[0] == "*" ) { // 如果計算完了 把結果加入 MUL_result 給write result 使用
            MUL_result.push_back( ALU_MUL[4] ) ; // RS1 1(result)
            MUL_result.push_back( to_string( stoi( ALU_MUL[1] ) * stoi( ALU_MUL[2] ) ) ) ;
            string temp(1,ALU_MUL[4][2]) ;
            MUL_RS[stoi(temp)-4].clear() ;
            ALU_MUL.clear() ;
        } // if
        else if ( ALU_MUL[0] == "/" ) {
            MUL_result.push_back( ALU_MUL[4] ) ;
            MUL_result.push_back( to_string( stoi( ALU_MUL[1] ) / stoi( ALU_MUL[2] ) ) ) ;
            string temp(1,ALU_MUL[4][2]) ;
            MUL_RS[stoi(temp)-4].clear() ;
            ALU_MUL.clear() ;
        } // if
        
        
    } // else if
    if ( ALU_MUL.empty() && ! next_ALU_MUL.empty() ) { // 如果現在buffer是空的 但是有新的進來
        if ( next_ALU_MUL[0] == "/" )
            gALU_MUL_cycle = div -1 ;
        else if( next_ALU_MUL[0] == "*" )
            gALU_MUL_cycle = mul -1 ;
        ALU_MUL = next_ALU_MUL ;
        next_ALU_MUL.clear() ;
    } // if
    

    // 倒數到0 如果倒數剩下0的話 -> 算完
    // 算完的結果要給WriteResult 跟 Calculate
} // Calculate

void Update_RAT( string reg, string value ) {
    RAT[reg] = "none" ;
} // Update_RAT

void Capture( string reg, string value ) {
    // update RS裡的
    // find all the reg to change the value
    for( int i = 0 ; i < 3 ; i ++ ) {
        if ( Add_RS[i].size() != 0 ) {
            for( int j = 1 ; j < 3 ; j ++ ) {
                if ( Add_RS[i][j] == reg ) {
                    Add_RS[i][j] = value ;
                } // if
            } //
        } // if
    } // for
    
    for( int i = 0 ; i < 2 ; i ++ ) {
        if ( MUL_RS[i].size() != 0 ) {
            for( int j = 1 ; j < 3 ; j ++ ) {
                if ( MUL_RS[i][j] == reg ) {
                    MUL_RS[i][j] = value ;
                } // if
            } //
        } // if
    } // for
    
} // Capture

void WriteResult() {
    string reg1, reg2 ;
    string value1, value2 ;
    if ( !ADD_result.empty() || !MUL_result.empty() ) {
        if ( !ADD_result.empty() ) {
            reg1 = ADD_result[0] ;
            value1 = ADD_result[1] ;
            for( auto i : RAT ) {
                if ( i.second == reg1 ) {
                    RF[i.first] = stoi( value1 ) ;
                    i.second = "none" ;
                } //
            } //
//            RF[reg1] = stoi( value1 ) ; // 寫回RF
            //更新RAT
            Update_RAT( reg1, value1 ) ;
            Capture( reg1, value1 ) ;
            //清掉result
            ADD_result.clear() ;
        } // if
        
        if ( !MUL_result.empty() ) {
            reg2 = MUL_result[0] ;
            value2 = MUL_result[1] ;
            for( auto i : RAT ) {
                if ( i.second == reg2 ) {
                    RF[i.first] = stoi( value2 ) ;
                    i.second = "none" ;
                } //
            } //
//            RF[reg2] = stoi( value2 ) ; // 寫回RF
            //更新RAT
            Update_RAT( reg2, value2 ) ;
            Capture( reg2, value2 ) ;
            //清掉result
            MUL_result.clear() ;
        } // if
        
    } // if
    else {
        
    } // else
    // ADD_resutl MUL_result
    // 1. update RAT  2. 寫回RF
    // Capture()
} // WriteResult

bool Is_RS_empty() {
    if ( Add_RS[0].empty() && Add_RS[1].empty() & Add_RS[2].empty() ) {
        if (  MUL_RS[0].empty() && MUL_RS[1].empty() )
            return true ;
    } // if
    
    return false ;
} // Is_RS_empty


void Perform() {
//    Output_ALL() ;
    // Issue -> Dispatch 不能同一cycle
    // Capture -> Dispatch 不能同一cycle
    // 以下動作實際上為同時發生 類似pipeline
    // 透過先Dispatch再Issue 避免在同一cycle執行
//    cout << Instruction.size() ;
    while( true ) {
        if ( Instruction.size() == 0 && Is_RS_empty() && ALU_ADD.empty() && ALU_MUL.empty() ) {
            break;
        }
            
        Dispatch() ; // 2 4
        Calculate() ; // Perform Buffer裡面的執行 (倒數執行的cycle) 2 3\ 4
        if ( Instruction.size() != 0 )
            Issue() ; // 1 2 3 4
        WriteResult() ; // 包含 capture

        
        // 1. update RAT  2. 寫回RF 3. update RS裡的
        Output_ALL() ;
        gCurrent_cycle ++ ;
    
    }
    
} // Perform



int main(int argc, const char * argv[]) {
    // insert code here...
    
    GetToken() ; // queue
    Init() ;
    
    
    Perform() ;

    Output_ALL() ;

    return 0;
}
