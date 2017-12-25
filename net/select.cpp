#include "select.hpp"

Select::Select()
{
	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);
	FD_ZERO(&error_fds);
	zero_sec.tv_sec  = 0;
	zero_sec.tv_usec = 0;
	max_fds = 0;
}
void Select::select(std::vector<std::pair<int,int> >& active_fds)
{
	fd_set rfds = read_fds;
	fd_set wfds = write_fds;
	fd_set efds = error_fds;
	// log_debug("is read set:%d",FD_ISSET(all_fds[0],&rfds));
	int n = ::select(max_fds+1,&rfds,&wfds,&efds,&zero_sec);
	if(n<0)
	{
		log_debug("select error!!!!:%d,%d,%s",n,errno,get_error_msg(errno));
	}
	int size = all_fds.size();
	for(int i=0;i<size;++i)
	{
		int fd = all_fds[i];
		int revents = 0;
		if(FD_ISSET(fd,&rfds))
			revents |= READ;
		if(FD_ISSET(fd,&wfds))
			revents |= WRITE;
		if(FD_ISSET(fd,&efds))
			revents |= ERROR;
		if(revents)
			active_fds.push_back(std::make_pair(fd,revents));
	}
}
void Select::add_handle(int fd,int events)
{
	
	assert(fd!=-1);
	assert(handles.find(fd) == handles.end());
	if(fd>max_fds)
		max_fds = fd;
	all_fds.push_back(fd);
	handles[fd] = all_fds.size()-1;
	
	if(events & READ)
		FD_SET(fd,&read_fds);
	if(events & WRITE)
		FD_SET(fd,&write_fds);
	if(events & ERROR)
		FD_SET(fd,&error_fds);
	log_debug("add_handle:fd:%d,event:%x",
		fd,events);
	// log_debug("is read!!%d",FD_ISSET(fd,&read_fds));
}
inline void Select::update_fd(int fd,int events,int e,struct fd_set* fds)
{
	int is_inset = FD_ISSET(fd,fds);
	if(is_inset)
	{
		if(!(events&e))
			FD_CLR(fd,fds);
	}
	else if(events&e)
	{
		FD_SET(fd,fds);
	}
}
void Select::update_event(int fd,int events)
{
	assert(fd!=-1);
	update_fd(fd,events,READ,&read_fds);
	update_fd(fd,events,WRITE,&write_fds);
	update_fd(fd,events,ERROR,&error_fds);

	log_debug("update_handle:fd:%d,event:%x",
		fd,events);
}
void Select::rm_handle(int fd)
{
	log_debug("will rm handle:%d",fd);
	auto find_it = handles.find(fd);
	assert(find_it!=handles.end());
	int idx = (*find_it).second;
	if(all_fds.size()>1 && idx!=all_fds.size()-1)
	{
		int last_fd = all_fds.back();
		handles[last_fd] = idx;
		all_fds[idx] = last_fd;
	}
	
	//clear events
	update_fd(fd,0,READ,&read_fds);
	update_fd(fd,0,WRITE,&write_fds);
	update_fd(fd,0,ERROR,&error_fds);
	all_fds.pop_back();
	handles.erase(find_it);
	if(all_fds.empty())
		max_fds = 0;
	else if(fd == max_fds)
		max_fds = (--handles.end())->first;
	log_debug("rm_handle:fd:%d",fd);
}