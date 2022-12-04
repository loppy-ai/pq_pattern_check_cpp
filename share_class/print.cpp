#include "print.h"

// ���ʕ\��
void printResult(const Param_Info* pi, const Chain_Info* ci)
{
    std::cout << "---------------------�A�����---------------------" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "|   �F   |������|  �{��  |�y�A�{��|" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "|   ��   |" << std::right << std::setw(6) << ci->getElementCountPerColor(Red) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci->getMagnificationPerColor(pi, Red) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci->getMagnificationPerColor(pi, Red) * 5.5 << "|" << std::endl;
    std::cout << "|   ��   |" << std::right << std::setw(6) << ci->getElementCountPerColor(Blue) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci->getMagnificationPerColor(pi, Blue) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci->getMagnificationPerColor(pi, Blue) * 6.2 << "|" << std::endl;
    std::cout << "|   ��   |" << std::right << std::setw(6) << ci->getElementCountPerColor(Green) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci->getMagnificationPerColor(pi, Green) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci->getMagnificationPerColor(pi, Green) * 4.5 << "|" << std::endl;
    std::cout << "|   ��   |" << std::right << std::setw(6) << ci->getElementCountPerColor(Yellow) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci->getMagnificationPerColor(pi, Yellow) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci->getMagnificationPerColor(pi, Yellow) * 5.5 << "|" << std::endl;
    std::cout << "|   ��   |" << std::right << std::setw(6) << ci->getElementCountPerColor(Purple) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci->getMagnificationPerColor(pi, Purple) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci->getMagnificationPerColor(pi, Purple) * 5.5 << "|" << std::endl;
    std::cout << "|���C���h| ---- |" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci->getMagnificationPerColor(pi, None) << "| ------ |" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
}

// �^�C���X�^���v�\��
std::string getTimeStamp()
{
	time_t timer;
	struct tm local_time;
    std::stringstream ss;

	timer = time(NULL);
	localtime_s(&local_time, &timer);
	ss << std::setfill('0') << std::right << std::setw(2) << local_time.tm_mon + 1 << "/"
        << std::setfill('0') << std::right << std::setw(2) << local_time.tm_mday << " " 
        << std::setfill('0') << std::right << std::setw(2) << local_time.tm_hour << ":"
        << std::setfill('0') << std::right << std::setw(2) << local_time.tm_min << ":"
        << std::setfill('0') << std::right << std::setw(2) << local_time.tm_sec;

	return ss.str();
}