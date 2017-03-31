#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

template <class T>
class k_means
{
public:
	//k 要聚类的类别个数,feature_length特征向量的长度
	k_means(int k,int feature_length);
	~k_means();
    
    //data是输入的二维数组,是数据mxn,m是样本个数,也就是sample_num,n是feature_length
    // assignTo长度是sample_num,代表每一个样本属于哪一个类,每个类从0开始标号
    void cluster(T** data,int* assignedTo,int sample_num,int max_iter=-1);

    // 根据计算点与center之间的距离,并更新该点属于哪一类
	T update_class(T* sample,int index);
	//计算点与点之间的距离
	T point_distance(T* x,T* y,int length);
	//将点分配到相应的center:计算距离,看哪个最近就将点分配到哪一个类
	T k_assignTo();
	//计算新的数据的中心
	void cal_center();
		//记录所有的点属于哪个类别
	int* m_assignedTo;
	T** m_center;
	T** m_center_old;
	int m_center_number;
	int m_feature_length;

};

template <class T>
k_means<T>::k_means(int k,int feature_length)
{
	m_center_number = k;
	m_feature_length = feature_length;

	m_center = new T*[m_center_number]();
	for (int i = 0; i < m_center_number; ++i)
	{
		m_center[i] = new T[m_feature_length]();
	}


	m_center_old = new T*[m_center_number]();
	for (int i = 0; i < m_center_number; ++i)
	{
		m_center_old[i] = new T[m_feature_length]();
	}

}

template <class T>
k_means<T>::~k_means()
{
	
	for (int i = 0; i < m_center_number; ++i)
	{
		delete [] m_center[i];
		delete [] m_center_old[i];
	}

	delete [] m_center;
	delete [] m_center_old;
}

template <class T>
T k_means<T>::point_distance(T* x,T* y,int length)
{
	T sums=0;
	T temps=0;
	for (int i = 0; i < length; ++i)
	{
		temps =x[i]-y[i];
		sums += sqrt(1.0*temps*temps);
	}
	return sums;
}

//计算到各个类别的距离并根据该距离判断属于哪个类
template <class T>
T k_means<T>::update_class(T* sample,int index)
{
	T* distance = new T[m_center_number]();
	T temp = 0;
	//point_distance();
	for (int i = 0; i < m_center_number; ++i)
	{
		distance[i] += point_distance(sample,m_center[i]);		
	}
	//m_assignedTo[index] = 0;
	T smallest = distance[0];
	for (int i = 1; i < m_center_number; ++i)
	{
		if (smallest>distance[i])
		{
			m_assignedTo[index] = i;
			smallest = distance[i];
		}
	}

	delete [] distance;
}

template <class T>
void k_means<T>::cal_center(T** data,int sample_num)
{
	for (int i = 0; i < m_center_number; ++i)
	{
		memcpy(m_center_old[i],m_center[i],sizeof(T)*m_feature_length);
		memset(m_center[i],0,sizeof(T)*m_feature_length);
	}
	int* count = new int[m_center_number]();
	for (int i = 0; i < sample_num; ++i)
	{
		for (int j = 0; j < m_center_number; ++j)
		{
			if (m_assignedTo[i]==i)
			{
				for (int k = 0; k < m_feature_length; ++k)
				{
					m_center[j][k] += data[i][k]; 
				}
				count[i]++;
			}
			
		}
	}

	for (int i = 0; i < m_center_number; ++i)
	{
		if (count[i])
		{
			for (int j = 0; j < m_feature_length; ++j)
			{
				m_center[i][j] = 1.0*m_center[i][j]/count[i]
			}
		}
	}

    T sums=0;
    for (int i = 0; i < m_center_number; ++i)
    {
    	sums += point_distance(m_center[i],m_center_old[i],m_feature_length);
    }

	delete [] count;

	if (sums)
	{
		return true;
	}else
	    return false;
}

//max_iter是允许的最大迭代次数,当不需要这个值,而是根据实际情况进行迭代直到center不再改变时,将这个值设置为-1
template <class T>
void k_means<T>::cluster(T** data,int* assignedTo,int sample_num,int max_iter)
{

	m_assignedTo = new int[sample_num]();
	//初始化中心位置
	for (int i = 0; i < m_center_number; ++i)
	{
		for (int i = 0; i < m_feature_length; ++i)
		{
			m_center[i][j] = data[i][j];
		}
	}

	bool isContinue=true;
	int iter=0;
	while(isContinue)
	{
		//计算每个点到中心的距离并判别属于哪个类
		for (int i = 0; i < sample_num; ++i)
		{
			update_class(data[i],i);
		}

		//重新计算中心
		isContinue = cal_center(data,sample_num)
		iter++;
		if (iter>max_iter && max_iter>0)
		{
			isContinue=false;
		}
	}

	memcpy(assignedTo,m_assignedTo,sizeof(int)*sample_num);

	delete [] m_assignedTo;

	return ;
}
