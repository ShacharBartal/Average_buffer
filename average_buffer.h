/*
 Shachar Bartal 305262016
 */

#ifndef average_buffer_h
#define average_buffer_h

/*-----------------------------------------------------------------------------------------------------------------------------*/

/*
 To save Space complexity, I will use a double linked list, and as long as the list is incomplete, I do not use unnecessary space.
 A double linked list will allow me to reach the relevant quarter when I have to, without wasting time, saving time complexity.
 */

template <class T>
class Node
{
public:
    T data;
    Node<T> *next, *prev;
};

/*-----------------------------------------------------------------------------------------------------------------------------*/


template <class T>
class AverageBuffer
{
    
private:

    /*
     long:
     SIZE = const value for the buffer size.
     numOfNodesEverInBuff = how many nodes ever added to buffer.
     quarter = what is the quarter of the current size.
     
     T:
     For the best time complexity, I will save each addSample() the relevant sum.
     sumOfForever = The sum of data ever added to the buffer.
     sumOfCurrent = Sum of current samples
     sumOfUpper/Lower will calculate the sum of samples in the upper/lower quarter.
     
     Node<T>*:
     oldest/newest = ptr to the oldest/newest sample
     upper/lowerQuarterPtrNpde = ptr to the node that end/starts the upper/lower node
     */
    
    const long SIZE;
    long numOfNodesEverInBuff, quarter;
    T sumOfForever, sumOfCurrent, sumOfUpper, sumOfLower;
    Node<T> *oldest, *newest, *upperQuarterPtrNode, *lowerQuarterPtrNode;
    
/*------------------------------------------------------------------------------------------*/
    
public:
        
    AverageBuffer(long s) : SIZE(s)
    {
        numOfNodesEverInBuff = quarter = 0;
        sumOfForever = sumOfCurrent = sumOfLower = sumOfUpper = 0;
        newest = oldest = upperQuarterPtrNode = lowerQuarterPtrNode = nullptr;
        
    }

/*------------------------------------------------------------------------------------------*/
    
    /*
     Add a new sample:
     1. Declare a new node and update the numOfNodesEverInBuff and sumOfCurrent/Forever.
     2. Check if it is the first node at buffer.
        If so - update *newest and *oldest to the same node.
        
        Else:
            1. Update *newest.
            2. If I already have 4 samples, and SIZE is large or equal to 4,  I have the first quarter values, so I call to updateQuarter() (explanation below).
                    *** I am calling updateQuarter() before I change the *oldest because maybe I need to delete the *oldest from quarter data. ***
            3. If there are more numOfNodesEverInBuff than the buffer size, I must:
                1. Subtract the oldest data from sumOfCurrent.
                2. Update the oldest node (and delete the previous one).
     */
    
    void addSample(T data){
        
        Node<T> *temp = new Node<T>;
        temp->data = data;
        temp->prev = nullptr;
        numOfNodesEverInBuff++;
        sumOfCurrent += data;
        sumOfForever += data;

        if (newest == nullptr)
            newest = oldest = temp;
        
        else{
            temp->next = newest;
            newest->prev = temp;
            newest = temp;
            
            if ( (SIZE >= 4) && (numOfNodesEverInBuff >= 4) )
                updateQuarter();
            
            if (numOfNodesEverInBuff > SIZE){
                
                sumOfCurrent -= oldest->data;
                oldest = oldest->prev;
                delete oldest->next;
                oldest->next = nullptr;
            }
        }
    }
    
/*------------------------------------------------------------------------------------------*/

    /*
     When numOfNodesEverInBuff>= 4, I need to start calculating the quarters.
     1. No matter what, I need to increase sumOfUpper with my new data.
     2. I need to check if it is the first time I've calculated a quarter.
        1. If it is the first time I should update 'quarter' variable, let upper/lower pointers hold the newes/lowest nodes, and increase sumOfLower. Done.
     
        2. Else, I have 2 conditions to check and update accordingly.
            1. Is the buffer full ? I will keep it inside bool sizeIsFull.
            2. Do I need increase 'quarter'? I will keep it inside bool numOfQuarterSamplesHasChanged.
     
            A. If sizeIsFull I need to update sumOfUpper/Lower and upper/lowerQuarterPtrNode.
            B.  Else, if numOfQuarterSamplesHasChanged = true I need to:
                1. Increase the number of the samples in each quarter (withouth reducing the oldest).
                2. Add to sumOfLower and update lower quarter pointer.
                
            3. Else, I need to reducing from sumOfUpper and update upper quarter pointer.
     
     Because I have some conditions to check, by which to take some steps, I also save boolean variables to the lower / upper quarters data and then I do not have to repeat rows.
     */
    
    void updateQuarter(){
        
        sumOfUpper += newest->data;
        
        if (numOfNodesEverInBuff == 4){
            quarter = 1;
            upperQuarterPtrNode = newest;
            lowerQuarterPtrNode = oldest;
            sumOfLower += lowerQuarterPtrNode->data;
        }
        
        else {
            
            bool sizeIsFull = (numOfNodesEverInBuff > SIZE);
            bool addTo_SumOfLower_andUpdate_lowerQuarterPtrNode = sizeIsFull , subFrom_sumOfUpper_andUpdate_upperQuarterNode = sizeIsFull;
            bool numOfQuarterSamplesHasChanged = (!sizeIsFull && (numOfNodesEverInBuff % 4 == 0));
            
            if (sizeIsFull)
                sumOfLower -= oldest->data;
            
            if (!sizeIsFull){
                
                if (numOfQuarterSamplesHasChanged){
                    addTo_SumOfLower_andUpdate_lowerQuarterPtrNode = true;
                    quarter = numOfNodesEverInBuff / 4;
                }
                
                else{
                    subFrom_sumOfUpper_andUpdate_upperQuarterNode = true;
                }
            }
            
            if (subFrom_sumOfUpper_andUpdate_upperQuarterNode){
                sumOfUpper -= upperQuarterPtrNode->data;
                upperQuarterPtrNode = upperQuarterPtrNode->prev;
            }
            
            if (addTo_SumOfLower_andUpdate_lowerQuarterPtrNode){
                lowerQuarterPtrNode = lowerQuarterPtrNode->prev;
                sumOfLower += lowerQuarterPtrNode->data;
            }
        }
    }
    
/*------------------------------------------------------------------------------------------*/
    
    T getAverage(){
        return ((SIZE == 0) ? 0 : (numOfNodesEverInBuff > SIZE) ? sumOfCurrent/SIZE : (numOfNodesEverInBuff == 0) ? 0 : sumOfCurrent/numOfNodesEverInBuff);
    }

/*------------------------------------------------------------------------------------------*/
    
    T getAverageForever(){
        return (numOfNodesEverInBuff == 0) ? 0 : sumOfForever / numOfNodesEverInBuff;
    }

/*------------------------------------------------------------------------------------------*/
    
    T getUpperQuarterAverage(){
        return quarter > 0 ? sumOfUpper/quarter : 0;
    }
    
/*------------------------------------------------------------------------------------------*/

    T getLowerQuarterAverage(){
        return quarter > 0 ? sumOfLower/quarter : 0;
    }
    
/*------------------------------------------------------------------------------------------*/

    /*
     To clear the list I have to run on all nodes, from the newest to the oldest and delete them.
     Next, I want to reset my variables.
     */
    
    void clear(){
        Node<T>* next = newest;
        
        while (next != nullptr){
            delete newest;
            next = next->next;
            newest = next;
        }
        
        oldest = newest = upperQuarterPtrNode = lowerQuarterPtrNode = nullptr;
        numOfNodesEverInBuff = quarter = 0;
        sumOfForever = sumOfCurrent = sumOfUpper = sumOfLower = 0;
    }
    
/*------------------------------------------------------------------------------------------*/

    ~AverageBuffer(){
        clear();
    }
    
/*------------------------------------------------------------------------------------------*/

};
#endif
