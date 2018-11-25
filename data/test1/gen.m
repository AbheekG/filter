function gen
	%%% Meta-data %%%
	%%%%%%%%%%%%%%%%%
	
	meta_fid = fopen ("meta.csv", "w");

	% n_epoch
	n_epoch = 10;
	fprintf (meta_fid, "%d\n", n_epoch);
	
	% n_dim
	n_dim = 1;
	fprintf (meta_fid, "%d\n", n_dim);
	
	% size of each dimension
	dim_size = [10000];
	for i = 1:n_dim
		fprintf (meta_fid, "%d", dim_size(i)+1);
		if i < n_dim
			fprintf (meta_fid, ",");
		else
			fprintf (meta_fid, "\n");
		end
	end

	% lower and upper bound of each dimension
	lower_bound = [-5];
	upper_bound = [5];
	for i = 1:n_dim
		fprintf (meta_fid, "%f,%f\n", lower_bound(i), upper_bound(i));
	end

	% Data along each dimension. Redundancy, size and bounds sufficient!
	for i = 1:n_dim
		X = lower_bound(i) : (upper_bound(i)-lower_bound(i))/dim_size(i) : upper_bound(i);
		for i = 1:length(X)
			fprintf (meta_fid, "%f", X(i));
			if i < length(X)
				fprintf (meta_fid, ",");
			else
				fprintf (meta_fid, "\n");
			end
		end
	end

	fclose (meta_fid);

	%%% Generating data %%%
	%%%%%%%%%%%%%%%%%%%%%%%

	input_fid = fopen ("input.csv", "w");

	% Parameters specific to this test
	Param = [
	0.4, 0.1, 0.5;
	0.4, 0.2, 0.4;
	0.3, 0.3, 0.4;
	0.3, 0.4, 0.3;
	0.2, 0.5, 0.3;
	0.2, 0.6, 0.2;
	0.1, 0.7, 0.2;
	0.1, 0.8, 0.1;
	0.0, 0.9, 0.1;
	0.0, 1.0, 0.0;
	];

	% Generating data
	for i = 1:n_epoch
		Y = X;
		for j = 1:length(X)
			Y(j) = func(X(j), Param(i,1), Param(i,2), Param(i,3));
		end

		for i = 1:length(Y)
			fprintf (input_fid, "%f", Y(i));
			if i < length(X)
				fprintf (input_fid, ",");
			else
				fprintf (input_fid, "\n");
			end
		end
	end

	fclose (input_fid);

end

function [y] = func(x, a, b, c)
	y = a*normpdf (x, -3, 1) + b*normpdf (x, -1, 1) + c*normpdf (x, 3, 1);
end
