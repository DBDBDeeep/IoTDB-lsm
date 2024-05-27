EXECUTABLE="./makeWorkload"

datasets = (data_c100000000_d0.05 data_c100000000_d0.1 data_c100000000_d0.2 data_c100000000_d0.3)


for dataset in "${datasets[@]}"; do
    echo "Running with dataset=${dataset} and "
    $EXECUTABLE $name $init_data_count $out_of_order_ratio
done