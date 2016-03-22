//
//  main.cpp
//  Anti-MacShop
//
//  Created by Francisco Aguilera on 3/30/15.
//  Copyright (c) 2015 Francisco Aguilera. All rights reserved.
//

#include <iostream>
#include <thread>
// Threadpool
#define BOOST_ASIO_HAS_MOVE
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
// Filesystem
#include <boost/filesystem.hpp>

static void parallel_remove_file(boost::asio::io_service & ioservice,
                                 const boost::filesystem::path & in_path,
                                 const std::string & with_name)
{
    if(exists(in_path))
    {
        bool isDir = is_directory(in_path);
        if(in_path.filename().string().find(with_name) != std::string::npos)
        {
            std::cout
            << "Found: "
            << in_path.string()
            << ", deleting..."
            << std::endl;
            
            isDir
            ? boost::filesystem::remove_all(in_path)
            : boost::filesystem::remove(in_path);
        }
        else
        {
            if(isDir)
            {
                boost::filesystem::directory_iterator end_itr;
                for(boost::filesystem::directory_iterator itr(in_path);
                    itr != end_itr;
                    ++itr )
                {
                    ioservice.post(boost::bind(parallel_remove_file,
                                               boost::ref(ioservice),
                                               boost::ref(itr->path()),
                                               boost::ref(with_name)));
                }
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    const std::string file_name("macshop");
    const std::string apps_dir("/Applications/");
    const std::string sf_exts_dir("~/Library/Safari/Extensions/");
    const std::string ff_exts_dir("~/Library/Application Support/Firefox/Profiles/");
    const std::string cm_exts_dir("~/Library/Application Support/Google/Chrome/");
    
    boost::asio::io_service ioservice;
    boost::thread_group threadpool;
    boost::asio::io_service::work work(ioservice);
    unsigned int threads = std::thread::hardware_concurrency();
    for(unsigned int i = 0; i < threads; ++i)
        threadpool.create_thread(boost::bind(&boost::asio::io_service::run,
                                             &ioservice));
    ioservice.post(boost::bind(parallel_remove_file,
                               boost::ref(ioservice),
                               boost::ref(apps_dir),
                               boost::ref(file_name)));
    
    ioservice.post(boost::bind(parallel_remove_file,
                               boost::ref(ioservice),
                               boost::ref(sf_exts_dir),
                               boost::ref(file_name)));
    
    ioservice.post(boost::bind(parallel_remove_file,
                               boost::ref(ioservice),
                               boost::ref(ff_exts_dir),
                               boost::ref(file_name)));
    
    ioservice.post(boost::bind(parallel_remove_file,
                               boost::ref(ioservice),
                               boost::ref(cm_exts_dir),
                               boost::ref(file_name)));
    threadpool.join_all();
    ioservice.stop();
    
    return 0;
}