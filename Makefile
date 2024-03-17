buildDebug : 
	mkdir -p build_debug && cd build_debug && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j8 && cp model ../
buildRelease :
	mkdir -p build_release && cd build_release && cmake -DCMAKE_BUILD_TYPE=Release -DTRACE_ON=OFF .. && make -j8 && cp model ../
regression: buildDebug
	find thirdParty/regression -type f -exec ./model -c config/RVH1.yaml -e {} \;
clean:
	rm -f model && rm -rf build_debug && rm -rf build_release
