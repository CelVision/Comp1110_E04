import csv

def analyze_campus_map(file_path):
    nodes = set()
    edges = []  
    adjacency = {}
    
    buildings_to_ignore = ["Yu Chun Keung Tam Shuk Yin Lecture Theatre"]

    print(f"正在读取文件: {file_path}...\n")

    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            reader = csv.reader(f)
            header = next(reader, None) # 跳过表头
            
            for row in reader:
                if len(row) < 2:
                    continue
                
                u = row[0].strip()
                v = row[1].strip()
                
                if not u or not v:
                    continue

                if u in buildings_to_ignore or v in buildings_to_ignore:
                    continue

                nodes.add(u)
                nodes.add(v)
                edges.append((u, v))
                
                if u not in adjacency: adjacency[u] = set()
                if v not in adjacency: adjacency[v] = set()
                
                adjacency[u].add(v)
                adjacency[v].add(u)

    except Exception as e:
        print(f"读取文件时出错: {e}")
        return

    # 1. 提取并打印所有节点
    print("="*60)
    print("1. 提取的所有节点名称 (按字母排序):")
    print("="*60)
    sorted_nodes = sorted(list(nodes))
    for node in sorted_nodes:
        print(f"{node}")
    print(f"\n总计节点数: {len(sorted_nodes)}\n")

    # 2. 统计检查每个点的邻居
    print("2. 节点邻居检查:")
    for node in sorted_nodes:
        neighbors = sorted(list(adjacency[node]))
        # 格式: Node : Neighbor1, Neighbor2, ...
        print(f"{node} : {', '.join(neighbors)}")
    print()

    # 3. 检查异常的单向通路
    print("3. 异常单向通路检查 (只有 A->B 但没有 B->A):")
    
    # 将边列表转换为集合以便快速查找
    edge_set = set(edges)
    one_way_anomalies = []
    
    for u, v in edge_set:
        # 检查反向边 (v, u) 是否存在
        if (v, u) not in edge_set:
            one_way_anomalies.append((u, v))
            
    if not one_way_anomalies:
        print("检查通过: 所有路径看起来都是双向的 (A->B 和 B->A 均存在)。")
    else:
        print(f"发现 {len(one_way_anomalies)} 条可能的单向路径 (数据缺失或单行道):")
        # 按起点排序打印
        for u, v in sorted(one_way_anomalies):
            print(f" [异常] {u}  -->  {v}  (缺少反向: {v} --> {u})")
            
    print("\n" + "="*60)
    print("分析完成")

if __name__ == "__main__":
    analyze_campus_map('Paths.txt')