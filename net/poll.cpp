#include "poll.hpp"
#include <iostream>

// int poll(struct pollfd *fds, nfds_t nfds, int timeout);
// struct pollfd {
//         int   fd;         /* file descriptor */
//         short events;     /* requested events */
//         short revents;    /* returned events */
//     };

// POLLIN There is data to read.

// POLLPRI
//        There is some exceptional condition on the file descriptor.
//        Possibilities include:

//        *  There is out-of-band data on a TCP socket (see tcp(7)).

//        *  A pseudoterminal master in packet mode has seen a state
//           change on the slave (see ioctl_tty(2)).

//        *  A cgroup.events file has been modified (see cgroups(7)).

// POLLOUT
//        Writing is now possible, though a write larger that the avail‐
//        able space in a socket or pipe will still block (unless O_NON‐
//        BLOCK is set).

// POLLRDHUP (since Linux 2.6.17)
//        Stream socket peer closed connection, or shut down writing
//        half of connection.  The _GNU_SOURCE feature test macro must
//        be defined (before including any header files) in order to
//        obtain this definition.

// POLLERR
//        Error condition (only returned in revents; ignored in events).
//        This bit is also set for a file descriptor referring to the
//        write end of a pipe when the read end has been closed.

// POLLHUP
//        Hang up (only returned in revents; ignored in events).  Note
//        that when reading from a channel such as a pipe or a stream
//        socket, this event merely indicates that the peer closed its
//        end of the channel.  Subsequent reads from the channel will
//        return 0 (end of file) only after all outstanding data in the
//        channel has been consumed.

// POLLNVAL
//        Invalid request: fd not open (only returned in revents;
//        ignored in events).

// When compiling with _XOPEN_SOURCE defined, one also has the follow‐
// ing, which convey no further information beyond the bits listed
// above:
// POLLRDNORM
//        Equivalent to POLLIN.

// POLLRDBAND
//        Priority band data can be read (generally unused on Linux).

// POLLWRNORM
//        Equivalent to POLLOUT.

// POLLWRBAND
//        Priority data may be written.

Poll::Poll(){
	log_debug("TEST:POLLIN:%0x,POLLOUT:%0x,POLLERR:%0x",
		POLLIN,POLLOUT,POLLERR
		);
}
void Poll::poll(std::vector<std::pair<int,int> >& active_handles)
{
	//fd:events
	int n = ::poll(&*(pollfds.begin()), pollfds.size(), time_out);
	if(n>0)
	{
		int enums = n;
		for(auto it=pollfds.begin();it!=pollfds.end() && enums>0;++it)
		{
			if(!it->revents)
				continue;
			int e = from_poll_event(it->revents);
			int fd = it->fd;
			active_handles.push_back(std::make_pair(fd,e));
			--enums;
			// log_debug("poll it!!fd:%d,e:%0x,revens:%0x,event:%0x",fd,e,it->revents,it->events);
		}
	}
	else if(n<0)
	{
		log_err("poll error:%d,%d,%s",n,errno,get_error_msg(errno));
	}
}
void Poll::select(std::vector<std::pair<int,int> >& active_handles)
{
	poll(active_handles);
}
void Poll::add_handle(int fd,int events)
{
	assert(fd!=-1);
	assert(handles.find(fd)==handles.end()); //can't find it!!

	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = to_poll_event(events);
	pollfds.push_back(pfd);
	handles[fd] = pollfds.size()-1;
	log_debug("[poll]add handle:fd:%d,events:%0x,pfd.events:%0x",
		fd,events,pfd.events);
}
void Poll::rm_handle(int fd)
{
	log_debug("will rm_handle:fd:%d",fd);
	auto find_it = handles.find(fd);
	assert(find_it!=handles.end()); //find it!!
	int idx = (*find_it).second;
	int size = pollfds.size();
	assert(size>0);
	if(size>1 && idx!=size-1)
	{
		struct pollfd last_fd = pollfds.back();
		handles[last_fd.fd] = idx;
		pollfds[idx] = last_fd;
	}
	pollfds.pop_back(); //move last;
	handles.erase(find_it);
	log_debug("rm_handle success!:fd:%d",fd);
}

void Poll::update_event(int fd,int events)
{
	assert(fd!=-1);
	auto find_it = handles.find(fd);
	assert(find_it!=handles.end());

	short e = to_poll_event(events);
	int idx = (*find_it).second;
	pollfds[idx].events = e;
}


