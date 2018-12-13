function gen
	format longE
	%%% Meta-data %%%
	%%%%%%%%%%%%%%%%%
	
	meta_fid = fopen ("meta.csv", "w");

	% n_epoch
	n_epoch = 10;
	fprintf (meta_fid, "%d\n", n_epoch);
	
	% n_dim
	n_dim = 2;
	fprintf (meta_fid, "%d\n", n_dim);
	
	% size of each dimension
	dim_size = [100, 100];
	for i = 1:n_dim
		fprintf (meta_fid, "%d", dim_size(i)+1);
		if i < n_dim
			fprintf (meta_fid, " ");
		else
			fprintf (meta_fid, "\n");
		end
	end

	% lower and upper bound of each dimension
	lower_bound = [0, 0];
	upper_bound = [100, 100];
	for i = 1:n_dim
		fprintf (meta_fid, "%e %e\n", lower_bound(i), upper_bound(i));
	end

	XX = {};
	% Data along each dimension. Redundancy, size and bounds sufficient!
	for i = 1:n_dim
		XX{i} = lower_bound(i) : (upper_bound(i)-lower_bound(i))/dim_size(i) : upper_bound(i);
		for j = 1:length(XX{i})
			fprintf (meta_fid, "%e", XX{i}(j));
			if j < length(XX{i})
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
	2, 0;
	5, 0;
	2, 5;
	0, 5;
	-5, 1;
	0, 1;
	2, 1;
	-1, -1;
	-5, 2;
	0, 2;
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

	% Generating data (2D)
	motion_ = [0,0];
	for i = 1:n_epoch
		motion_ = motion_ + Motion(i,:);
		coeff = 0;

		% TODO. only 2 dim
		YY = zeros(length(XX{1}), length(XX{2}));
		for j1 = 1:length(XX{1})
			for j2 = 1:length(XX{2})
				YY(j1,j2) = func([XX{1}(j1), XX{2}(j2)], ...
					Param(i,1), Param(i,2), Param(i,3), motion_);
				coeff = coeff + YY(j1,j2);
			end
		end

		for j = 1:n_dim
			coeff = (upper_bound(j)-lower_bound(j)) * coeff;
			coeff = coeff / dim_size(j);
		end

		for j1 = 1:length(XX{1})
			for j2 = 1:length(XX{2})
				YY(j1,j2) = YY(j1,j2) / coeff;
				fprintf (sensor_fid, "%e", YY(j1,j2));
				if ( (j1 < length(XX{1})) || (j2 < length(XX{2})) )
					fprintf (sensor_fid, " ");
				else
					fprintf (sensor_fid, "\n");
				end
			end
		end

		figure; surf (XX{1}, XX{2}, YY);
	end

	fclose (sensor_fid);

	input("Waiting to finish...");
	close all;

end

function [y] = func(X, a, b, c, M)
	X = X - M;
	y = a * normpdf (X(1), 40, 2) * normpdf (X(2), 80, 5) + ...
		b * normpdf (X(1), 30, 5) * normpdf (X(2), 30, 5) + ...
		c * normpdf (X(1), 70, 10) * normpdf (X(2), 20, 1) ;
end
