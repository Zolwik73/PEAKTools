mod PCANBasic;
#[path = "06_TimerWrite.rs"]
mod TimerWrite;

#[tokio::main]
async fn main() {
    TimerWrite::run().await;
}
