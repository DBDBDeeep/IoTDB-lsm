EXECUTABLE="./makeDataset"

name = data
init_data_count=100000000
out_of_order_ratios=(0.05 0.1 0.2 0.3) # 예: out of order 비율


for out_of_order_ratio in "${out_of_order_ratios[@]}"; do
    echo "Running with initDataCount=${init_data_count} and outOfOrderRatio=${out_of_order_ratio}"
    $EXECUTABLE $name $init_data_count $out_of_order_ratio
done

echo "make dataSet completed."