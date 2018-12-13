function plot_data2(file)
	test_path = "data/test3/";

	files = dir([test_path, "cdf-*"]);
	file_names = {};
	for i = 1:length(files)
		file_names{i} = files(i).name;
	end
		
	file_names = {};
	file_names{1} = "pdf-neural-network-500.csv";

	cdfs = {};
	for i = 1:length(file_names)
		cdfs{i} = dlmread([test_path, file_names{i}], " ");
	end

	
	for i = 1:size(cdfs{1},1)
		figure;
		% hold on;
		for j = 1:length(file_names)
			data = zeros(101,101);
			for j1 = 1:101
				for j2 = 1:101
					data(j1,j2) = cdfs{j}(i, (j1-1)*101 + j2 );
				end
			end
			surf (1:101, 1:101, data);
		end
		% hold off;
		legend (file_names);
	end
	
	input("Waiting to finish...");
	close all;
end 