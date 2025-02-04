function [H, H_fq, estimates, gps_data, NFrames] = load_estimates_lm(fid, NTx, NFrames_max, NFrames, version)
% 
% EMOS Single User Import Filter with low memory usage
%
% [H, H_fq, estimates, gps_data, NFrames] = load_estimates_lm(fid, NTx, NFrames_max, NFrames, version)
%
% Parameters:
% fid               - file descriptor of EMOS data file. Must be opened in
%                     read mode: fid = fopen('....EMOS','r');
% N_Tx              - Number of Tx antennas
% NFrames_max       - Maximum number of frames to read 
%                     (Use Inf to get up to the maximum file contents)
% Nframes           - Number of frames already read from the file 
%                     (Use return value of previous call)
% version           - see load_estimates for details
%
% Returns:
% H                 - Time domain channel matrix of size [NRx,NTx,NDelay,NFrames]
%                     The estimates of different users are aligned in time
% H_fq              - Frequency domain channel matrix of size [NRx,NTx,NFreq,NFrames]
% estimates         - A structure array containing timestamp, etc
% gps_data          - A structure array containing gps data
% NFrames           - the number of read estimates

% Author: Leonardo Cardoso and Florian Kaltenberger 
% Copyright: Eurecom Sophia Antipolis

% Version History
%   Date      Version   Comment
%   20070917  0.1       Created
%   20071116  0.2       Added fields framestamp_tx and crc_status in the
%                       dumpfile. However, the crc_status is not working
%                       yet, so we still use the pdu_errors field to
%                       calculate the err_ind.
%                       Also more than one file can be read now at the same
%                       time.
%   20071121  0.3       crc_status is now giving right values, so we can
%                       use it. However, the framestamp_tx is now buggy.
%                       Therefore we recalculate it here.
%   20071127  0.4       Removed previous bugs in dumpfile.

if nargin < 5
    version = Inf;  % We assume the latest version
end
NFreq = 256;
NZFreq = 160;
NRx = 2;

% Logfile structure: 
%  - 100 entries of type fifo_dump_emos (defined in phy_procedures_emos.h)
%  - 1 entry of type gps_fix_t defined in gps.h
TIMESTAMP_SIZE = 8;
FRAMESTAMP_TX_SIZE = 4;
PDU_ERRORS_SIZE = 4;
RX_POWER_SIZE = 2;
N0_POWER_SIZE = 2;
RX_RSSI_SIZE = 2;
NB_OF_OFDM_CARRIERS = 256;
NB_ANTENNAS = 2;
COMPLEX16_SIZE = 4;
MAC_PDU_SIZE = 140;
CRC_STATUS_SIZE = 4;
NO_ESTIMATES_DISK = 100;

if version >= 0.2
    FIFO_DUMP_EMOS_SIZE = ...
        TIMESTAMP_SIZE + ...
        FRAMESTAMP_TX_SIZE + ...
        PDU_ERRORS_SIZE + ...
        RX_POWER_SIZE * NB_ANTENNAS + ...
        N0_POWER_SIZE * NB_ANTENNAS + ...
        RX_RSSI_SIZE * NB_ANTENNAS + ...
        NB_ANTENNAS * NB_OF_OFDM_CARRIERS * COMPLEX16_SIZE + ...
        NB_ANTENNAS * NB_OF_OFDM_CARRIERS * COMPLEX16_SIZE + ...
        MAC_PDU_SIZE + ...
        CRC_STATUS_SIZE;
else
    FIFO_DUMP_EMOS_SIZE = ...
        TIMESTAMP_SIZE + ...
        PDU_ERRORS_SIZE + ...
        RX_POWER_SIZE * NB_ANTENNAS + ...
        N0_POWER_SIZE * NB_ANTENNAS + ...
        RX_RSSI_SIZE * NB_ANTENNAS + ...
        NB_ANTENNAS * NB_OF_OFDM_CARRIERS * COMPLEX16_SIZE + ...
        NB_ANTENNAS * NB_OF_OFDM_CARRIERS * COMPLEX16_SIZE + ...
        MAC_PDU_SIZE;
end


DOUBLE_SIZE = 8;
INT_SIZE = 4;
GPS_FIX_T_SIZE = INT_SIZE + 16 * DOUBLE_SIZE;

CHANNEL_BUFFER_SIZE = NO_ESTIMATES_DISK * FIFO_DUMP_EMOS_SIZE + GPS_FIX_T_SIZE;
    
% Preallocate data vectors
chan0 = zeros(NB_OF_OFDM_CARRIERS,NFrames_max);
chan1 = zeros(NB_OF_OFDM_CARRIERS,NFrames_max);
% estimates.sch_symbol0 = zeros(NB_OF_OFDM_CARRIERS,NFrames);
% estimates.sch_symbol1 = zeros(NB_OF_OFDM_CARRIERS,NFrames);
estimates.timestamp = zeros(1,NFrames_max);
estimates.timestamp_tx = zeros(1,NFrames_max);
estimates.framestamp_tx = zeros(1,NFrames_max);
estimates.pdu_errors = zeros(1,NFrames_max);
estimates.err_ind = false(1,NFrames_max);
estimates.rx_power = zeros(NB_ANTENNAS,NFrames_max);
estimates.n0_power = zeros(NB_ANTENNAS,NFrames_max);
estimates.rx_rssi_dBm = zeros(NB_ANTENNAS,NFrames_max);
estimates.mac_pdu = char(zeros(MAC_PDU_SIZE,NFrames_max));
estimates.flag = char(zeros(1,NFrames_max));
estimates.Hnorm = zeros(1,NFrames_max);

gps_data.timestamp = zeros(1,floor(NFrames_max/100));
gps_data.mode = zeros(1,floor(NFrames_max/100));
gps_data.ept = zeros(1,floor(NFrames_max/100));
gps_data.latitude = zeros(1,floor(NFrames_max/100));
gps_data.longitude = zeros(1,floor(NFrames_max/100));
gps_data.rest = zeros(12,floor(NFrames_max/100));

NFrames_start = NFrames;

k = 1;
l = 1;

    while ~feof(fid) && k <= NFrames_max

    %     temp    = fread(fid,TIMESTAMP_SIZE,'uchar'); % Header variable
    %     
    %     if(strcmp(char(temp), HEADER))
    %         break
    %     end

        % Read Timestamp Data
        temp    = fread(fid,TIMESTAMP_SIZE,'uchar'); % Timestamp variable
        if isempty(temp) && feof(fid)
            break
        end
        tt=0;
        temp=temp/256;
        for k2=1:TIMESTAMP_SIZE
            tt=(tt+temp(9-k2))*256;
        end
        estimates.timestamp(k)=tt/1e9; % to get the timestamp in sec

        if version>=0.2
            % Read tx frame no
            estimates.framestamp_tx(k) = fread(fid,1,'uint');
        end

        % Read PDU Errors
        estimates.pdu_errors(k) = fread(fid,1,'uint');

        % Read RX Power
        estimates.rx_power(:,k) = fread(fid,NB_ANTENNAS,'short');

        % Read Noise Power
        estimates.n0_power(:,k) = fread(fid,NB_ANTENNAS,'short');

        % Read Noise Power
        estimates.rx_rssi_dBm(:,k) = fread(fid,NB_ANTENNAS,'short');

        % Read channel estimates
        [data,count]    = fread(fid,NB_OF_OFDM_CARRIERS*NB_ANTENNAS*2,'short'); % read
        if (count ~= NB_OF_OFDM_CARRIERS*NB_ANTENNAS*2)
            warning('not all data read')
            break
        end

        % Create the receive signal
        chan0(:,k) = data(1:2:(NB_OF_OFDM_CARRIERS*2)) + j * data(2:2:(NB_OF_OFDM_CARRIERS*2));
        chan1(:,k) = data(((NB_OF_OFDM_CARRIERS*2)+1):2:(4*NB_OF_OFDM_CARRIERS)) + j * data(((NB_OF_OFDM_CARRIERS*2)+2):2:(4*NB_OF_OFDM_CARRIERS)) ;
        
        % calculate the mean squared Frobenius norm of H
        estimates.Hnorm(k) = (sum(abs(chan0(:,k)).^2) + sum(abs(chan1(:,k)).^2))/(NZFreq/NTx);

        % Read extra OFDM symbol
        [data,count]    = fread(fid,NB_OF_OFDM_CARRIERS*NB_ANTENNAS*2,'short'); % read
        if (count ~= NB_OF_OFDM_CARRIERS*NB_ANTENNAS*2)
            warning('not all data read')
            break
        end

        % Create the receive signal
        %estimates.sch_symbol0(:,k) = data(1:2:(NB_OF_OFDM_CARRIERS*2)) + j * data(2:2:(NB_OF_OFDM_CARRIERS*2));
        %estimates.sch_symbol1(:,k) = data(((NB_OF_OFDM_CARRIERS*2)+1):2:(4*NB_OF_OFDM_CARRIERS)) + j * data(((NB_OF_OFDM_CARRIERS*2)+2):2:(4*NB_OF_OFDM_CARRIERS)) ;

        % Read MAC PDU
        estimates.mac_pdu(:,k) = fread(fid,MAC_PDU_SIZE,'uchar');
        tt=0;
        temp=estimates.mac_pdu(1:8,k)/256;
        for k2=1:TIMESTAMP_SIZE
            tt=(tt+temp(9-k2))*256;
        end
        estimates.timestamp_tx(k)=tt/1e9; % to get the timestamp in sec

        if version>=0.3
            estimates.err_ind(k) = (fread(fid,1,'int') == -1);
        else
            if version>=0.2
                dummy = fread(fid,1,'int');
            end
            if k > 1
                estimates.err_ind(k) = (((estimates.pdu_errors(k) - estimates.pdu_errors(k-1)) > 0));
            end
        end

        if version<0.2
            estimates.framestamp_tx(k)=round(tt*3e-6/8); 
        elseif version<0.4 && estimates.err_ind(k) && k>1
            estimates.framestamp_tx(k) = estimates.framestamp_tx(k-1)+1;
        end        
        
        estimates.flag(k) = estimates.mac_pdu(17,k);

        k = k+1;
        NFrames = NFrames+1;

        %read GPS data
        if (mod(NFrames,NO_ESTIMATES_DISK)==0)
            gps_data.timestamp(l) = fread(fid,1,'double');
            gps_data.mode(l) = fread(fid,1,'int');
            gps_data.ept(l) = fread(fid,1,'double');
            gps_data.latitude(l) = fread(fid,1,'double');
            gps_data.longitude(l) = fread(fid,1,'double');
            gps_data.rest(:,l) = fread(fid,12,'double');

            l=l+1;
            %estimates.gps_data(k) = gps_data;
        end

    end

    NFrames_read = NFrames - NFrames_start;
    
    H_fq = complex(zeros(NRx,NTx,NFreq/NTx,NFrames_read));
    H_fq(1,:,:,:) = reshape(chan0(:,1:NFrames_read),NTx,NFreq/NTx,NFrames_read);
    H_fq(2,:,:,:) = reshape(chan1(:,1:NFrames_read),NTx,NFreq/NTx,NFrames_read);
    
    H = ifft(H_fq,[],3);
    
    % remove zero carriers from Frequency response
    NZFreq_ind = [(176/NTx+1):(256/NTx) 1:(80/NTx)]; % Non-zero frequency indices 
    H_fq = H_fq(:,:,NZFreq_ind,:);


