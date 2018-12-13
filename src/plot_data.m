function plot_data(file)
	test_path = "data/test1/";

	files = dir([test_path, "cdf-*"]);
	file_names = {};
	for i = 1:length(files)
		file_names{i} = files(i).name;
	end
	
	cdfs = {};
	for i = 1:length(file_names)
		cdfs{i} = dlmread([test_path, file_names{i}], " ");
	end

	legend_data = {"DGF", "GF", "Original", "NN", "PF", "PolyF"};

	
	for i = 1:size(cdfs{1},1)
		figure;
		hold on;
		for j = 1:length(file_names)
			plot (cdfs{j}(i,:));
		end
		hold off;
		legend (legend_data);
	end
	
	input("Waiting to finish...");
	close all;
end 