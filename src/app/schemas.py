from pydantic import BaseModel, Field
from typing import Optional, List

class HealthResponse(BaseModel):
    status: str
    version: str
    engine: str

class ProposalEvaluationRequest(BaseModel):
    proposal_id: int = Field(..., description="Уникальный ID инициативы")
    proposal_text_raw: str = Field(..., min_length=10, description="Текст предложения или ТЭО")
    author_did_hash: int = Field(..., description="Хэш DID автора")

class ProposalEvaluationResponse(BaseModel):
    is_passed: bool
    law_score: float
    fin_score: float
    eco_score: float
    deepfake_fraud_detected: bool
    evaluation_summary: str

class DebateAnalysisRequest(BaseModel):
    initiative_id: int
    forum_comments: List[str]

class DebateAnalysisResponse(BaseModel):
    initiative_id: int
    consensus_score: float
    key_concerns: List[str]
    is_deadlock_risk: bool