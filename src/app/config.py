import os
from pydantic_settings import BaseSettings

class Settings(BaseSettings):
    """
    Конфигурация окружения Python ИИ-Оракула (FastAPI) для экосистемы TRANSCEPTOR.
    """
    APP_NAME: str = "TRANSCEPTOR AI Oracle"
    VERSION: str = "2026.1.0"
    DEBUG: bool = False
    
    # Порты и Сервер
    PORT: int = 8000
    HOST: str = "0.0.0.0"
    
    # Пороги Agora AI Oracle
    LAW_THRESHOLD: float = 0.85
    FIN_THRESHOLD: float = 0.80
    ECO_THRESHOLD: float = 0.75
    
    # Интеграции C++ ядра
    CORE_LIBRARY_PATH: str = os.getenv("CORE_LIBRARY_PATH", "/app/libcore.so")
    
    class Config:
        env_file = ".env"

settings = Settings()