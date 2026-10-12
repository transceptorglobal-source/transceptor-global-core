# ====================================================================
# TRANSCEPTOR: SEMANTIC AI-ORACLE COGNITIVE RUNTIME (OPTIMIZED)
# TARGET: GPU-OPTIMIZED NLP INFRASTRUCTURE (GERMANY MIX)
# ====================================================================

FROM python:3.10-slim-bullseye

# Установка базовых системных утилит компиляции для scikit-learn
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Создание изолированной группы и пользователя для ИИ-контура
RUN groupadd -r ai_oracle && useradd -r -g ai_oracle -m -s /sbin/nologin aiuser

WORKDIR /home/aiuser/oracle_runtime

# Копирование файла детерминированных зависимостей
COPY requirements.txt .

# Установка пакетов машинного обучения без сохранения локального кэша pip
RUN pip install --no-cache-dir --upgrade pip \
    && pip install --no-cache-dir -r requirements.txt

# Перенос когнитивного кода ИИ-Оракула и мостов ядра
COPY --chown=aiuser:ai_oracle main.py .
COPY --chown=aiuser:ai_oracle core_bridge.py .
COPY --chown=aiuser:ai_oracle satellite_bridge.py .
COPY --chown=aiuser:ai_oracle sbt_decay_bridge.py .
COPY --chown=aiuser:ai_oracle consensus_mediator.py .

# Переключение на непривилегированную среду нулевого доверия
USER aiuser

# Порт асинхронного API-шлюза ИИ-Оракула
EXPOSE 8000

# Проверка состояния контейнера
HEALTHCHECK --interval=30s --timeout=5s --start-period=5s --retries=3 \
  CMD curl -f http://localhost:8000/api/v1/health || exit 1

ENV PYTHONUNBUFFERED=1 \
    PYTHONDONTWRITEBYTECODE=1

# Прямой вызов uvicorn
ENTRYPOINT ["uvicorn", "main:app", "--host", "0.0.0.0", "--port", "8000"]