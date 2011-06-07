% Copyright (C) 2011
% Author: Lawrence Murray <lawrence.murray@csiro.au>
% $Rev$
% $Date$

% -*- texinfo -*-
% @deftypefn {Function File} hist_mcmc (@var{in}, @var{invar}, @var{coord}, @var{burn}, @var{interval})
%
% Plot histogram of parameter samples output by mcmc program.
%
% @itemize
% @bullet{ @var{in} Input file. Gives the name of a NetCDF file output by
% mcmc.}
%
% @bullet{ @var{invar} Name of variable from input file to plot.
%
% @bullet{ @var{coord} (optional) Vector of spatial coordinates of zero
% to three elements, giving the x, y and z coordinates of a
% component of @var{invar} to plot.}
%
% @bullet{ @var{rang} (optional) Vector of indices of samples to
% plot. All samples plotted if not specified.
% @end itemize
% @end deftypefn
%
function hist_mcmc (in, invar, coord, rang)
    % constants
    THRESHOLD = 5e-3; % threshold for bin removal start and end
    BINS = 20;

    % check arguments
    if (nargin < 2 || nargin > 5)
        print_usage ();
    end
    if nargin < 3
        coord = [];
    end
    if nargin < 4
        rang = [];
    end
    if !check_coord (coord)
        error ('coord should be a vector with at most three elements');
    end

    % input file
    nci = netcdf (in, 'r');
    P = length (nci('np'));
    if length (rang) == 0
        rang = [1:P];
    end
    
    % read samples
    data = read_var (nci, invar, coord, rang, 1);
    ncclose (nci);
    
    % bin
    [nn,xx] = hist(data, BINS);
    
    % clean up outlier bins from either end
    mask = nn > THRESHOLD*length(data);
    found = find(mask, length(mask), "first");
    if length(found) > 1
        % have outliers
        first = found(1);
        last = find(mask, length(mask), "last")(end);
        if last == first
            last = last + 1;
        end
        mask(first:last) = 1; % only want to remove from start and end
        xx = xx(mask);
        nn = nn(mask);
    end
    
    % recompute reference range to restore full number of bins
    [nn,xx] = hist(data, xx(1):(xx(end)-xx(1))/(BINS-1):xx(end));
    [mm,yy] = hist(data, xx);
    % ^ first above should be with reference range
            
    % scale
    xsize = max(xx) - min(xx); % range of x values in histogram
    xdelta = xsize / 100; % res for prior distro plot
    ysize = length(data) / BINS; % average bar height

    % prior
    %x = min([ xx, truth(i) ]):xdelta:max([ xx, truth(i) ]);
    %if i == 3
    %    y = normpdf(x, mu0(i), sigma0(i));
    %else
    %    y = lognpdf(x, mu0(i), sigma0(i));
    %end
    %peak = max(max(nn) / sum(xsize/BINS*mm), max(y));
                        
    % plot
    h = bar(xx,mm / sum(xsize/BINS*mm), 1.0); % normalised histogram
    set(h, 'FaceColor', fade(watercolour(2),0.5), 'EdgeColor', watercolour(2));
end
