//input taken as nums array 

// Input1: nums = [3,1,5,8]
// Output1: 167


// Input2: nums = [1,5]
// Output2: 10


class Solution {
public:
    int maxCoins(vector<int>& nums) {

        nums.insert(nums.begin(),1);
        nums.push_back(1);
        
        int size = nums.size();
        
        vector<vector<int>> dp(size+1,vector<int>(size+1,-1));
        
        return solve(nums,1,size-2,dp);
    }
    
protected:
    int solve(vector<int> &nums,int i,int j,vector<vector<int>> &dp){
        if(i>j){
            return 0;
        }
        
        if(dp[i][j] != -1){
            return dp[i][j];
        }
        
        int ans=INT_MIN;
        
        
        for(int k=i;k<=j;k++){
            int calculated_ans = nums[i-1]*nums[k]*nums[j+1];
            
            int temp = calculated_ans + solve(nums,i,k-1,dp) + solve(nums,k+1,j,dp);
            
            ans = max(ans,temp);
        }
        
        return dp[i][j] = ans;
    }
};