#!/usr/bin/env bash
# 清理 SmartGround 的编译产物与测试截图
# 用法: bash tools/clean.sh [--all]
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

echo "[1/4] 清理测试截图"
find build -name "*.png" -type f -delete 2>/dev/null || true
find . -maxdepth 1 -name "*.png" -type f -delete 2>/dev/null || true

echo "[2/4] 清理测试可执行文件"
find build/tests -maxdepth 1 -name "*.exe" -type f -delete 2>/dev/null || true
find build/tests -maxdepth 1 -name "*.a" -type f -delete 2>/dev/null || true

echo "[3/4] 清理 QtCreator 残留"
rm -rf .qtcreator build-tests 2>/dev/null || true

if [ "${1:-}" = "--all" ]; then
    echo "[4/4] 全量清理 build 目录"
    rm -rf build 2>/dev/null || true
else
    echo "[4/4] 保留 build 目录 (加 --all 可全量删除)"
fi

echo "完成。当前占用:"
du -sh build 2>/dev/null || echo "  build 目录已不存在"
