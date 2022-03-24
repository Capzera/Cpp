/*
问题描述：P0253
给你一个会议时间安排的数组 intervals ，
每个会议时间都会包括开始和结束的时间 intervals[i] = [starti, endi] ，返回 所需会议室的最小数量 。
示例 1：
输入：intervals = [[0,30],[5,10],[15,20]]
输出：2
示例 2：
输入：intervals = [[7,10],[2,4]]
输出：1
提示：
1 <=?intervals.length <= 104
0 <= starti < endi <= 106
*/
class Solution {
public:
    int minMeetingRooms(vector<vector<int>>& intervals) {
		int n=intervals.size(),sptr=0,eptr=0;
        vector<int> start,end;
        for (auto& i:intervals) start.push_back(i[0]),end.push_back(i[1]);
        sort(start.begin(),start.end());
        sort(end.begin(),end.end());
        while(sptr<n){
			if (start[sptr++]>=end[eptr]) eptr++;
		}
        return sptr-eptr;
    }
};