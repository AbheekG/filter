function gen
	format longE
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
	dim_size = [1000];
	for i = 1:n_dim
		fprintf (meta_fid, "%d", dim_size(i)+1);
		if i < n_dim
			fprintf (meta_fid, " ");
		else
			fprintf (meta_fid, "\n");
		end
	end

	% lower and upper bound of each dimension
	lower_bound = [-5];
	upper_bound = [5];
	for i = 1:n_dim
		fprintf (meta_fid, "%e %e\n", lower_bound(i), upper_bound(i));
	end

	% Data along each dimension. Redundancy, size and bounds sufficient!
	for i = 1:n_dim
		X = lower_bound(i) : (upper_bound(i)-lower_bound(i))/dim_size(i) : upper_bound(i);
		for j = 1:length(X)
			fprintf (meta_fid, "%e", X(j));
			if j < length(X)
				fprintf (meta_fid, " ");
			else
				fprintf (meta_fid, "\n");
			end
		end
	end

	fclose (meta_fid);

	%%% Generating motion data %%%
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	motion_fid = fopen ("motion.csv", "w");

	% Parameters specific to this test
	Motion = [
	0;
	0.5;
	0.5;
	0.5;
	-0.5;
	-0.5;
	1;
	0.5;
	1;
	1;
	];

	% Generating data
	for i = 1:n_epoch
		for j = 1:n_dim
			fprintf (motion_fid, "%e", Motion(i,j));
			if j < n_dim
				fprintf (motion_fid, " ");
			else
				fprintf (motion_fid, "\n");
			end
		end
	end

	fclose (motion_fid);

	%%% Generating sensor data %%%
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	sensor_fid = fopen ("sensor.csv", "w");

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
	motion_ = 0;
	for i = 1:n_epoch
		motion_ = motion_ + Motion(i);
		coeff = 0;

		Y = X;
		for j = 1:length(X)
			Y(j) = func(X(j), Param(i,1), Param(i,2), Param(i,3), motion_);
			coeff = coeff + Y(j);
		end

		% TODO. only for 1 dim.
		coeff = (upper_bound(1)-lower_bound(1)) * coeff;
		coeff = dim_size(1) / coeff;

		for j = 1:length(Y)
			Y(j) = Y(j) * coeff;
			fprintf (sensor_fid, "%e", Y(j));
			if j < length(X)
				fprintf (sensor_fid, " ");
			else
				fprintf (sensor_fid, "\n");
			end
		end

		figure; plot (X, Y);
	end

	fclose (sensor_fid);

	input("Waiting to finish...");
	close all;

end

function [y] = func(x, a, b, c, motion_)
	y = a*normpdf (x - motion_, -3, 1) + ...
		b*normpdf (x - motion_, -1, 1) + ...
		c*normpdf (x - motion_, 3, 1);
end
