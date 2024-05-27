EXECUTABLE="./makeDataset"

dataSetName="data"
init_data_count=50000000
out_of_order_ratios=(0.1 0.2 0.3) # 예: out of order 비율


for out_of_order_ratio in "${out_of_order_ratios[@]}"; do
    echo "Running with initDataCount=${init_data_count} and outOfOrderRatio=${out_of_order_ratio}"
    $EXECUTABLE $dataSetName $init_data_count $out_of_order_ratio
done

echo "make dataSet completed."