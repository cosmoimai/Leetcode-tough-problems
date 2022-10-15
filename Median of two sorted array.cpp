Median of two sorted array : leetcode

// code

class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        vector<int> nums;
        for(int i=0; i<nums1.size(); i++) nums.push_back(nums1[i]);
        for(int i=0; i<nums2.size(); i++) nums.push_back(nums2[i]);
        
        sort(nums.begin(), nums.end());
        
        int mid = nums.size()/2;
        double ans;
        
        if(nums.size()%2 == 0) ans = double(nums[mid] + nums[mid - 1])/2;
        else ans = nums[mid];
        
        return ans;
    }
};
