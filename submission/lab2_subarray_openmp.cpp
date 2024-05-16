#include <iostream>
#include <vector>
#include <limits>
#include <omp.h>

int maxSubArraySum(const std::vector<int>& nums) {
    int n = nums.size();
    int max_sum = std::numeric_limits<int>::min();
    int current_sum = 0;

    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        current_sum = std::max(nums[i], current_sum + nums[i]);
        #pragma omp critical
        {
            max_sum = std::max(max_sum, current_sum);
        }
    }

    return max_sum;
}

int main() {
    std::vector<int> nums = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    int result = maxSubArraySum(nums);
    std::cout << "Maximum sum of contiguous subarray: " << result << std::endl;
    return 0;
}
